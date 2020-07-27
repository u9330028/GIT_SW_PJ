# 파이썬은 정규 표현식을 지원하기 위해 re(regular expression의 약어) 모듈을 제공한다
import re
import os

# 빈 딕셔너리인 초기화
frequency = {}
count_word_pairs = []
# 1) kaggle에서 제공받은 위키피디아 문장 데이터셋 (wikisent2.txt)파일을 문자열 변수에 저장
# 2) 파일을 읽기(r)와 텍스트(t) 모드로 open('wikisent2.txt', mode='rt', encoding='utf-8')
readfile = './wikisent2.txt' 
f = open(readfile, 'r')

# 3) 더이상 읽을 줄이 없을때까지 한 줄씩 데이터를 읽어 들인다
lcnt = 0
# for line in f:
while True:
    # 4) 다음과 같이 lower() 함수를 사용해 문서 내의 모든 문자를 소문자로 바꿉니다.
    line = f.readline()
    lcnt += 1
    if not line: break 
    text_string = line.lower()
    print(text_string)
    # 5) 문자열 명령어인 split()을 사용하여 문장을 단어로 나눈다
    readwords = text_string.split()
    for w in readwords:
        # 6) 문자가 아닌경우는 버린다
        if w.isalpha() == False:
            continue

        # 7) get(word, 0)에서 0은 해당 단어 키가 존재하지 않으면 기본으로 0을 돌려줍니다. 
        # 8) 만약 존재하면 값이 1씩 증가되므로 각 단어를 카운트합니다.
        count = frequency.get(w,0)
        frequency[w] = count + 1 
    
# 9) 파일을 Close (파일 핸들 닫기) 
f.close()

# 10) 아래 방법으로 키를 볼 수 있습니다.
frequency_list = frequency.keys()
# 11) 단어와 빈도(텍스트 파일에서 나타난 횟수)를 구하기 위해 아이템 리스트에 있는 단어와 개수를 출력하는 내용입니다.
for words in frequency_list:
    print(words, frequency[words])

# 12) 가장 많이 등장한 단어의 역순으로 정렬한다 
count_word_pairs= sorted(frequency.items(), reverse=True, key=lambda item: item[1])

# 가장 많이 등장한 단어를 빈도순으로 정렬된 데이터를 SetWordCount.txt에 저장하는 기능을 수행한다
# 1) 저장파일을 추가 모드('a') Open (파일 핸들 열기)
setfile = './SetWordCount.txt'
if os.path.isfile(setfile):
  os.remove(setfile)

idx = 0
f = open(setfile,'a')
f.write("=====================================================================================================================================================\n")
for key, value in count_word_pairs:
    # 2) 파일 쓰기 format (Key, Value)
    wt_line = "{0} : {1}\n".format(key, value)
    # 3) 파일 쓰기
    f.write(wt_line)
    idx += 1

    if (idx >= 1000): 
        print(wt_line, "word cnt = " + str(idx))
        break

f.write("=====================================================================================================================================================\n")
# 4) 저장파일 Close (파일 핸들 닫기)
f.close()