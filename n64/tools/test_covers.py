from pathlib import Path
import json,urllib.request,subprocess,tempfile
r=Path(__file__).resolve().parents[2];fe=r/'ps5/frontend';n=r/'n64';p=n/'build/cover-test';(p/'host').mkdir(parents=True,exist_ok=True)
url='https://raw.githubusercontent.com/Globaleliteee/Gaming_Console_Covers_Art_Repository/main/covers/nintendo/N64/Super%20Mario%2064%20%28U%29%20Front.jpg'
data=urllib.request.urlopen(url,timeout=30).read();assert data[:2]==b'\xff\xd8';(p/'test-cover.jpg').write_bytes(data);print('Repository JPEG downloaded:',len(data),'bytes (AI-assisted)',flush=True)
subprocess.run(['clang++-18','-std=c++20','-O1','-ffunction-sections','-fdata-sections','-I'+str(n/'src'),'-I'+str(fe),str(n/'tools/test_covers.cpp'),str(fe/'fe_covers.cpp'),str(fe/'fe_text.cpp'),str(fe/'fe_i18n.cpp'),'-Wl,--gc-sections','-pthread','-o',str(p/'host/cover-test')],check=True)
with tempfile.TemporaryDirectory()as cache:subprocess.run([str(p/'host/cover-test'),str(p/'test-cover.jpg'),cache],check=True)

