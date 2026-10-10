"""(AI-assisted) Host validation using original generated MIPS code; no commercial ROMs."""
from pathlib import Path
import subprocess,concurrent.futures,tempfile,zipfile,hashlib
project=Path(__file__).resolve().parents[2];app=project/'n64';core=app/'third_party/parallel-n64';stage=app/'tools';out=app/'build/host-test';out.mkdir(parents=True,exist_ok=True)
sources=subprocess.check_output(['make','-s','-f',str(stage/'core-sources.mk'),'list'],cwd=core,text=True).splitlines()
flags=subprocess.check_output(['make','-s','-f',str(stage/'core-sources.mk'),'flags'],cwd=core,text=True).splitlines();flags=[('-I'+str(core/x[2:]))if x.startswith('-I')else x for x in flags]
fingerprint=hashlib.sha256((' '.join(flags)).encode())
for p in sorted(core.rglob('*')):
 if p.is_file() and p.suffix in ('.h','.c','.cpp','.inc'):fingerprint.update(p.read_bytes())
stamp=out/'fingerprint';digest=fingerprint.hexdigest();force=not stamp.exists() or stamp.read_text()!=digest
def compile(name):
 p=core/name;o=out/Path(name).with_suffix('.o');o.parent.mkdir(parents=True,exist_ok=True)
 if force or not o.exists() or o.stat().st_mtime<p.stat().st_mtime:
  subprocess.run(['clang++-18' if p.suffix=='.cpp' else 'clang-18','-O2','-fPIC','-fcommon','-w','-std=c++11' if p.suffix=='.cpp' else '-std=gnu11',*flags,'-c',str(p),'-o',str(o)],check=True)
 return str(o)
with concurrent.futures.ThreadPoolExecutor(max_workers=4)as pool:objects=list(pool.map(compile,sources))
subprocess.run(['clang++-18','-shared','-o',str(out/'parallel.so'),*objects,'-pthread','-lz','-lm','-Wl,--no-undefined'],check=True);stamp.write_text(digest)
base=['clang++-18','-std=c++20','-O2','-I'+str(app/'src'),'-I'+str(app/'include'),'-I'+str(project/'ps5/frontend')]
subprocess.run([*base,str(stage/'test_n64_session.cpp'),str(app/'src/core_session.cpp'),str(out/'parallel.so'),'-Wl,-rpath,'+str(out),'-pthread','-o',str(out/'session')],check=True)
subprocess.run([*base,str(stage/'test_roms.cpp'),str(app/'src/games.cpp'),str(app/'src/library_cache.cpp'),str(app/'src/controller_bindings.cpp'),str(project/'ps5/frontend/fe_nfs.cpp'),'-lz','-pthread','-o',str(out/'roms')],check=True)
with tempfile.TemporaryDirectory()as temporary:
 root=Path(temporary);games=root/'games';games.mkdir();rom=bytearray(4096);rom[:4]=bytes.fromhex('80371240');rom[64:80]=bytes(range(16));(games/'test.z64').write_bytes(rom)
 v=bytearray(rom);w=bytearray(rom)
 for i in range(0,len(v),2):v[i:i+2]=v[i:i+2][::-1]
 for i in range(0,len(w),4):w[i:i+4]=w[i:i+4][::-1]
 (games/'test.v64').write_bytes(v);(games/'test.n64').write_bytes(w);(games/'bad.z64').write_bytes(bytes(4096))
 for name,method in [('test.zip',zipfile.ZIP_DEFLATED),('stored.zip',zipfile.ZIP_STORED)]:
  with zipfile.ZipFile(games/name,'w',method)as archive:archive.writestr('nested/test.z64',rom)
 with zipfile.ZipFile(games/'multiple.zip','w')as archive:archive.writestr('a.z64',rom);archive.writestr('b.n64',w)
 with zipfile.ZipFile(games/'empty.zip','w')as archive:archive.writestr('readme.txt','no ROM')
 corrupt=bytearray((games/'stored.zip').read_bytes());corrupt[60]^=1;(games/'corrupt.zip').write_bytes(corrupt)
 subprocess.run([str(out/'roms'),str(games)],check=True,timeout=30)
 subprocess.run([str(out/'session'),str(root/'saves')],check=True,timeout=45)
print('N64 host integration tests passed (AI-assisted)')
