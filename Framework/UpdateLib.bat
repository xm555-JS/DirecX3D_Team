

////xcopy	/�ɼ�    .�����ؾ��� ���϶Ǵ�����			.������� ����. 

xcopy   /y		.\Engine\Bin\Engine.lib					.\Reference\Librarys\
xcopy   /y		.\Engine\ThirdPartyLib\*.lib			.\Reference\Librarys\
xcopy   /y		.\Engine\Bin\Engine.dll					.\Client\Bin\
xcopy   /y		.\Engine\Bin\PhysX_64.dll				.\Client\Bin\
xcopy   /y		.\Engine\Bin\PhysXCommon_64.dll			.\Client\Bin\
xcopy   /y		.\Engine\Bin\PhysXFoundation_64.dll		.\Client\Bin\
xcopy   /y/s	.\Engine\Public\*.*						.\Reference\Headers\
xcopy   /y/s	.\Engine\Bin\ShaderFiles\*.*			.\Client\Bin\ShaderFiles\