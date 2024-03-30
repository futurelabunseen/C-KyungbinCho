## 기능 2. 기본 공격, 기본 상호작용
### 작업 완료

[무기 기반 작업] <br/>
ㄴ Weapon(베이스) - GASWeapon(상속) - GASGun(상속) 구조 <br/>
ㄴ 장착, 탈착 기능 <br/>
ㄴ 총기류(GASGun) 탄창을 관리하는 AmmoSet <br/>

[기본 공격 2개] <br/>
ㄴ BP_PulseRifle 펄스 소총, BP_BioticRifle 생체 소총 <br/>
ㄴ 단발 발사 GA <br/>
ㄴ 연사 발사 GA (단발 GA 를 AbilitiyTask_Repeat 으로 반복 트리거) <br/>
ㄴ TargetActor_SingleLineTrace 로 피격 판정 <br/>

[기본 상호작용] <br/>
ㄴ 체력을 관리하는 HealthSet <br/>
ㄴ GE 로 데미지 처리 (아직 멀티 적용 전이므로 제대로 동작은 X) <br/>

[위젯] <br/>
ㄴ 임시 빌보드 체력바 <br/>

[기타] <br/>
ㄴ IMC, IA 를 열거형과 함께 데이터 애셋으로 묶어 관리 <br/>
ㄴ 입력 매핑, 바인딩 관련 코드를 PlayerController, Character => HeroComponent, InputComponent 로 이동 <br/>
ㄴ TArray<GA>, TArray<GE> 를 한번에 적용하는 함수를 Character => ASC 로 이동 <br/>
ㄴ 콘솔 커맨드 등 매크로 상수에는 GX_ 접두사 추가
