# -*- mode: python ; coding: utf-8 -*-

block_cipher = None


a = Analysis(['NAVS_SUPPORT_SERVICE.py'],
             pathex=['C:\\Users\\Jon Coffield\\Documents\\GitHub\\NAVS-Alpha1\\charlie2'],
             binaries=[],
             datas=[],
             hiddenimports=['win32timezone'],
             hookspath=[],
             runtime_hooks=[],
             excludes=[],
             win_no_prefer_redirects=False,
             win_private_assemblies=False,
             cipher=block_cipher,
             noarchive=False)
pyz = PYZ(a.pure, a.zipped_data,
             cipher=block_cipher)
exe = EXE(pyz,
          a.scripts,
          a.binaries,
          a.zipfiles,
          a.datas,
          [],
          name='NAVS_SUPPORT_SERVICE',
          debug=False,
          bootloader_ignore_signals=False,
          strip=False,
          upx=True,
          upx_exclude=[],
          runtime_tmpdir=None,
          console=True )
