# homework week2 

## 각 조의 프로젝트에 push 가 일어나면 자동으로 deploy 가 일어나도록 한다.

## 방법
1. 각 조 조장은 github 의 주소를 강사에게 공유한다.
2. github project setting > webhook 에 다음의 주소를 등록한다.(http://13.124.119.160:8080/github-webhook/  )
3. push 이벤트를 발생시켜 deploy 가 되는 것을 확인.

## success or fail
1. 숙제 마감은 12/07 00:00 까지 입니다.
2. http://robin.com:888X/blackjack/index.html 에서 확인
3. port 는 888 + 자신의 조 입니다 (1조는 8881, 2조는 8882)

## 주의
1. <매우중요> application.yml 에 server.port 를 꼭 수정하세요. (다른 조와 충돌 안되게) -> 수정 안해도 됩니다. 
2. root 의 README 를 보고 꼭! /etc/hosts 를 수정해주세요.
3. webhook 주소의 마지막에 슬러시문자 (/) 를 꼭 넣어주세요.
4. http://13.124.119.160:8080 에서 확인해볼 것 

## 문의
1. rokim@riotgames.com 로 메일 주세요
2. 메일 제목에는 [강원대] 를 prefix 로 붙여주세요.
