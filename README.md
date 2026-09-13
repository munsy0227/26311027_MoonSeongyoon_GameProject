# 마지막 단서

**학번:** 26311027

**이름:** 문성윤

**과목명:** 객체지향프로그래밍Ⅰ

**과제명:** 3주차 - 그래픽 리소스 적용 및 게임 시작 구현

귀신을 보고 대화할 수 있는 서준이 폐교에 남은 단서를 찾고, 같은 비주얼 노벨 화면에서 적대 귀신과 전투하는 짧은 게임을 기획한다. 최종 기준 해상도는 1920×1440이며, 5일 개발 목표로 장소 2곳·전투 1회로 제한한다.

## 문서

- [통합 기획서 PDF](docs/GameDesign.pdf)
- [게임 기획서 원본](doc/게임기획서.md)
- [시나리오 구성](doc/시나리오.md)
- [시나리오 1부](doc/시나리오_1부_서준.md)
- [시나리오 2부](doc/시나리오_2부_서준.md)
- [화면 구성](doc/화면구성.md)
- [리소스 목록](doc/ResourceList.md)

## 프로젝트

Visual Studio C++ / glc2d 프로젝트: `26311027-MoonSeongyoon-VN`


## 3주차 실행 범위

시작 화면 → 프롤로그 → 서연과 첫 대화 → 보관함 조사 → 그림자의 경고 → 깨진 바닥 → 교실 기록 조사 → 그림자의 정체와 마지막 전투 직전까지 이어진다. 기존 시나리오의 139개 대사와 시스템 안내를 142개 표시 페이지로 구성했다. 복도·교실 배경과 서연·그림자 이미지를 사용한다.

- 시작: 시작 버튼 또는 Enter/Space
- 대사 진행: 다음 버튼, 대화창 클릭, Enter/Space
- 선택: 버튼 클릭, 숫자 1/2, 또는 위·아래 방향키로 선택 후 Enter
- 타이틀 복귀: 플레이 중 Esc 또는 타이틀로 버튼
- 종료: 시작 화면에서 Esc 또는 게임 종료 버튼
- 재시작: HP 100, Life 3, 점수 0, 첫 대사로 초기화

## 빌드 및 실행

1. Visual Studio 2022에서 `26311027-MoonSeongyoon-VN/26311027-MoonSeongyoon-VN.sln`을 연다.
2. NuGet 패키지를 복원한다 (`glc2d 0.1.0.7`, `Microsoft.DXSDK.D3DX 9.29.952.8`).
3. **Debug / x64**로 빌드한다.
4. F5로 실행하거나 `26311027-MoonSeongyoon-VN/x64/Debug/26311027-MoonSeongyoon-VN.exe`를 연다.

빌드가 PNG·TTF와 MP3 리소스를 실행 폴더의 `resource/`에 복사한다. MP3는 실행용 영문 별칭으로 복사하며 원본 파일명은 유지한다. 실행 파일을 따로 옮길 때는 옆의 `resource/`와 DLL을 함께 옮긴다. 소스는 기존 CP949 인코딩을 유지하며 Visual Studio에서 이 인코딩으로 연다.

글꼴은 사용자가 교체한 에이투지체(A2Z 6 SemiBold)를 사용한다. 메인 BGM은 `Sayuri Loop1_01.mp3`이며 시작·대화 화면에서 재생한다. `Iwan Gabovitch - Dark Ambience Loop.mp3`는 전투 BGM, `qubodupPunch01.mp3`는 공격 효과음, `Replenish.mp3`는 회복 효과음으로 프로젝트에 등록했다. `click4.mp3`는 버튼·대사 진행 효과음으로 연결했다. 음원별 출처와 사용 조건은 [ResourceList.md](doc/ResourceList.md)에 기록했다.

- [최신 클래스 구성 및 구현 명세](doc/게임기획서.md#17-3주차-클래스-구성과-현재-구현-범위)
- [실제 리소스·출처·준비 현황](doc/ResourceList.md)
- [검증 및 제출 기록](doc/Week03.md)

게임 전체가 완성된 상태는 아니다. 실제 전투·전투 후 대사·최종 결말과 후속 장면의 사운드 연결은 남아 있다. 음원·폰트 출처와 사용 조건은 [ResourceList.md](doc/ResourceList.md)에 기록했다. 강의 PDF의 전체 3주차 기준 충족 여부와 이번 요청 범위를 구분한다.

대사 원본은 `doc/시나리오_1부_서준.md`, `doc/시나리오_2부_서준.md`이다. `python tools/build_story_data.py`로 전투 전 구간의 C++ 데이터를 재생성하고 `--check`로 일치 여부를 검사한다. `powershell -File tests/Run-SmokeTests.ps1`은 다섯 선택지의 32가지 조합과 입력·보상·표시 상태를 검사한다.
