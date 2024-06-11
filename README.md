# IoT시스템응용및프로그래밍

**2024 1학기 아두이노 프로젝트**

🤱**팀장** : 박성재[20]

👪**팀원** : 김서율[20], 김지찬[20], 유지석[21]

## 넌 내꺼야! 나만의 비바리움 스마트 사육장 [2024-05-10 ~ 2024-06-11]

**1. 프로젝트 개요**

|             **프로젝트명**              | 넌 내꺼야! 나만의 비바리움 스마트 사육장                     |
| :-------------------------------------: | :----------------------------------------------------------- |
|      **프로젝트 소개 및 제안배경**      | 코로나 판데믹 이후로 실내 생활과 여가 활동이 증가하면서 반려 동물이 아닌 ‘반려식물', ‘반려생물'을 키우는 취미를 즐기는 사람들이 늘어나고 있다. <br /> 비바리움(Vivarium)이란 곤충‧파충류‧식물 등을 관찰할 수 있는 생태계를 유리 어항 안에 재현하는 취미생활을 말한다. 비바리움은 사육하는 생물에 따라 종류가 나뉜다. 식물만 키우는 경우는 테라리움(Terrarium)이라고 한다. 곤충을 비롯해 무척추동물은 인섹타리움(Insectarium)이라고 한다. 그 밖에도 양서류를 사육하는 팔루다리움(Paludarium), 희귀 난초를 사육하고 관리하는 오키다리움(Orchidarium)도 있다. 최근에는 이끼만을 전문적으로 사육하는 이끼 테라리움도 마니아 사이에서 큰 인기를 얻고 있다.       <br />출처 : 팝콘뉴스(http://www.popcornnews.net)<br /> 이번 프로젝트는 비바리움이 취미로 자리잡음에 따라 환경에 민감한 파충류를 사육하는 사육장 환경을 IoT기술을 활용하여 자동으로 관리하고 실시간으로 모니터링하는 것을 목표로 한다. 환경에 민감한 생물임을 감안하여, 처음 파충류 사육을 시작하는 사람들에게 사육하는 생물의 환경 정보를 제공하여 파충류 사육에 대한 진입 장벽을 낮추고자 한다.<br /> 더 나아가 스마트폰 어플리케이션을 통해 사육장 환경 정보를 실시간으로 모니터링하고 변경할 수 있도록 함으로써 파충류 사육을 즐기는 사람들끼리 환경 정보를 공유 할 수 있는 커뮤니티를 구축할 것이다. |
|     **제품/서비스 및 기술의 개요**      | 사육장 내부에는 실시간 모니터링에 필요한 온/습도 센서가 있고 환경을 조성하는 엑츄에이터로는 UVB램프, 초음파 가습기, 팬, LED조명 등이 사용된다.<br />앱으로 구현된 UI/UX를 이용해 센서에서 받은 정보를 WiFi를 통해 데이터를 앱으로 실시간으로 확인할 수 있으며, 언제 어디서든 모든 엑츄에이터를 조절 및 관리할 수 있다. 생물별 사육환경 데이터를 사용자들 간의 공유 및 적용할 수 있다. |
| **종래 기술과의 차별성 및 기술의 장점** | 현재 존재하는 기술들은 주로 한 가지 기능에만 집중하는 경향이 있습니다. 그러나 우리의 제품은 온도와 습도 두 가지를 모두 관리할 수 있는 차별성을 가지고 있습니다. 이 기술의 주요 장점은 온도와 습도를 수동으로 조절하지 않고, 어플리케이션을 통해 조절할 수 있다는 것입니다. 이로써 사용자들은 언제 어디서든 자신의 파충류를 관리할 수 있습니다. |
|         **기대효과 및 시장성**          | 국내 파충류 거래는 사육 난이도와 종의 특이성에 따라 가격이 결정된다. 희귀한 종은 일반적으로 사육이 어려워 가격이 높다. 따라서 자동으로 사육환경을 관리하는 스마트 사육장을 통해 더 적은 노력으로 고품질의 파충류 사육이 가능하다. <br /> 증가하고 있는 파충류 사육을 취미로 즐기고자 하는 사람들의 진입장벽이 낮아질 것으로 예상된다. 이 제품을 통해 파충류 사육 및 분양으로 이윤을 창출하는 업자들은 더욱 효율적으로 사업환경을 조성할 수 있을 것으로 전망된다. 따라서 이 제품은 파충류 사육을 취미 또는 사업으로 삼는 사람들에게 모두 유용하며, 이는 파충류 사육 분야 시장을 더욱 성장시킬 것으로 예상된다.<br /> 더 나아가 파충류 뿐만 아니라 곤충, 식물 등 다양한 생물을 손쉽게 키우며, 누구나 비바리움을 조성할 수 있는 획기적인 제품이 될 것이다. |



**2. 프로젝트 내용**

| **구분** | **기능**                             | **설명**                                                     |
| -------- | ------------------------------------ | ------------------------------------------------------------ |
| H/W      | 사육장 환경 정보를 제공              | 온/습도 센서를 사용하여 센싱 값을 아두이노에 전달            |
| H/W      | 사육장 환경을 조절 및 관리           | UVB램프, 초음파 가습기, 팬, LED조명을 사용하여 조절 및 관리  |
| S/W      | 실시간 모니터링 및 환경을 조성, 관리 | 스마트폰 어플을 사용하여 사육장을 실시간 모니터링 및 환경을 조성, 관리 |

![스크린샷 2024-06-10 오후 1.46.39](/Users/seongjae/Desktop/스크린샷 2024-06-10 오후 1.46.39.png)

 **3. 최소 기능 제품/서비스** 

  가. 제작 개발 주요 내용

| **품목**                                              | **활용계획**                                                 |
| ----------------------------------------------------- | ------------------------------------------------------------ |
| 온습도센서 [DHT11]                                    | 사육 환경에 필요한 온도/습도 정보를 얻음                     |
| 초음파 센서                                           | 가습기 모듈에 사용되는 물탱크의 양을 체크                    |
| 가습기 모듈 DIY Micro USB 용 (모듈+분무) [SZH-HUM002] | 초음파 분무 원리를 이용해 사육장 내부 습도 조절              |
| 5V 3528 Flexible LED 5M/1롤 [SZH-LD304]               | 사육장 내부 조명을 구성                                      |
| 4체널 릴레이 모듈                                     | 외부 전원을 사용하여 엑츄에이터를 키고 끄는데 사용           |
| 4010 DC12V 6,000RPM 팬모터 [SZH-GNP512]               | 온습도 조절을 위한 팬                                        |
| 5v 워터펌프                                           | 토양과 물컵에 물을 공급                                      |
| 아두이노 호환 12V 1.5A KC 인증 아답터                   | 12v 외부전원 사용을 위한 아답터                        |
| TIP120 트랜지스터                                     | 12v 팬 모터 속도 조절, 12v PTC 히터를 제어하기 위한 PWM 구성 |
| Arduino UNO R4 WiFi                                   | 여러가지 센서의 값을 받고, 그 값을 WiFi를 사용하여 앱에 전달앱에서 전달한 값을 엑츄에이터에 전달하여 작동 |
| PTC 히터                                              | 사육장 환경을 알맞게 조절                                    |
| 어플리케이션                                          | 센서를 통해 얻은 정보를 표시 및 각종 엑츄에이터 작동 및 조절 |

 나. MVP 예상 도면 설명

| **예상 결과물 이미지**                                       | **설명**                                                     |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| **![img](https://lh7-us.googleusercontent.com/Z_GPFIbAl1aMCMzNk-AL9APH2Xxlr5tmEcq--NfPC3kCZ7DAh86wY-EfWNJFT6XNdN9hRxXzblZuj8KVj8FPq5Awph6HJn4zH9mIbk05ajs1hRxyIi_d3lvje7fHH2QHsUkCASKc_2r7CqgpdOi93g)** | 파충류들을 위한 스마트 사육장 이다. 어플리케이션으로 설정을 해서, 조명을 On/Off를 할 수 있으며, 온도 조절도 가능하다. 파충류들의 생활 공간에 필요한 습도를 위해서 설정된 습도로 조절을 해주고, 물을 저장해둔 공간에 물이 부족하다면 어플리케이션에 알림기능을 통해서 물을 추가적으로 저장하도록 한다. |

## **개발일지**

    1일차 : 2024.05.10
    장소 : 온라인, 디스코드, LIVE SHARE
    한 일 : 아두이노 메소드 코딩

    2일차 : 2024.06.05
    장소 : 미래혁신관
    한 일: 아두이노, 어플, 데이터베이스 연결 / 아두이노 센서 및 엑츄에이터 작동 테스트

    3일차 : 2024.06.09
    장소 : 미래혁신관
    한 일 : 아두이노에서 측정한 센서값을 데이터베이스에 저장, 어플에서 실시간 모니터링이 가능하게 UI 디자인

    4일차 : 2024.06.11 완성
    장소 : 미래혁신관
    한 일 : 사육장 환경 조성, 사육장과 센서 위치에 맞춰 가공해온 아크릴판에 센서 및 엑츄에이터 조립
          영상촬영, 깃허브, 보고서 제출

## **2024.06.11 (화) 작품 완성**

**1차, 2차 물품 구매를 마치고 필요한 물품을 마트에서 더 구매**

![4CF30B5F-52EA-43E1-AC4E-99E53EB11B36](https://github.com/SungJae01/IoT_FinalProject/assets/88194064/ff19ae9b-6703-4bc1-8b59-6ad5199e89f7)

![IMG_0033](https://github.com/SungJae01/IoT_FinalProject/assets/88194064/25771e24-cfc1-47a2-b1c1-088ef0074a4b)


 사육장 코너에서 더 꾸미고싶어서 산 물품들

## **제작 사진**

![IMG_0853](https://github.com/SungJae01/IoT_FinalProject/assets/88194064/9e5932b3-1c1e-467c-9174-f77b94fced6e)


**토양에 수분을 공급하기 위한 테프론 튜브배치**

![IMG_0036](https://github.com/SungJae01/IoT_FinalProject/assets/88194064/769b122d-2ba2-4e33-81b7-97c8625901a2)


**토양 수분 공급과 물컵 물공급을 위한 물탱크의 워터펌프 2개 부착**

![IMG_0054](https://github.com/SungJae01/IoT_FinalProject/assets/88194064/a87eb619-8275-492e-b0a1-12d6c0426885)


**워터펌프와 PTC 히터를 사육장에 부착**

![AA704C89-0CD8-4A5C-AA7D-275FC766D89B](https://github.com/SungJae01/IoT_FinalProject/assets/88194064/1d683707-cd5f-45cb-8b1b-fdf674c450c4)


**물탱크의 물양을 측정하기 위한 초음파 센서를 물탱크 뚜껑에 부착**

![IMG_0068](https://github.com/SungJae01/IoT_FinalProject/assets/88194064/dc34cbc1-e593-4d21-ad4c-0fb869d740c5)


**물컵의 양을 측정하기 위한 수위센서 부착**

![IMG_0056](https://github.com/SungJae01/IoT_FinalProject/assets/88194064/f525e738-7660-41a1-9e86-304c16dfc49f)


**사육장 뚜껑을 CAD를 사용하여 설계 및 아크릴판 가공**

![KakaoTalk_Photo_2024-06-12-01-12-58](https://github.com/SungJae01/IoT_FinalProject/assets/88194064/e1dddd7b-fa8f-43db-8b0b-340732716d6a)

![KakaoTalk_Photo_2024-06-12-01-12-45](https://github.com/SungJae01/IoT_FinalProject/assets/88194064/8f054e5a-db62-4b70-92c9-7e72186ea0de)

**12v LED바를 사육장 뚜껑에 부착**

![IMG_0066](https://github.com/SungJae01/IoT_FinalProject/assets/88194064/acf89e9a-b1a5-4311-bb04-974dd386d134)


**외부전원을 사용하여 12v PTC 히터와 12v 팬모터를 작동시키기 위한 TIP 120 트랜지스터 회로**

![IMG_0059](https://github.com/SungJae01/IoT_FinalProject/assets/88194064/cc8181bf-c862-4afd-aa95-a8609020c1c0)


**모바일 설계**

<img width="1630" alt="스크린샷 2024-06-12 오전 1 16 09" src="https://github.com/SungJae01/IoT_FinalProject/assets/88194064/c7c4ab0a-bd05-4c0e-bff7-1f70c886fa88">

<img width="1630" alt="스크린샷 2024-06-12 오전 1 16 37" src="https://github.com/SungJae01/IoT_FinalProject/assets/88194064/04de3156-7235-4b19-9872-6a8b81c178ad">


**완성 사진**

![5441C64E-C7ED-4B24-B123-20AC5694A700](https://github.com/SungJae01/IoT_FinalProject/assets/88194064/abb898d6-27f2-4248-9f3f-5be366174f90)

## **시현 영상 링크**

**시현 영상 1**

https://youtu.be/xIIneYoTvyk?si=R9eSZCuWhs-XnCay

**시현 영상 2 (추가)**

https://youtu.be/xzf4mcFUeLM
