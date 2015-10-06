#-*-coding:utf-8-*-
#author:chenshuchao
#email: shuchao.me@gmail.com
#date : 2015/2/26
import urllib2,urllib,smtplib,time
from email.mime.text import MIMEText
from email.header import Header
import re

class Spider:
    """
    本爬虫抓取北邮人论坛羽毛球版首页活动报名贴信息，当有新的报名贴时自动发送邮件提醒
    """
    def __init__(self):
        #configuration
        self.minute = 10 #每 10 分钟抓取一次
        self.domain = "http://bbs.byr.cn"
        self.keyword = "活动报名帖" #抓取的关键词
        self.myUrl =  "http://bbs.byr.cn/board/Badminton?_uid=guest" # 通过抓包看真实请求的 url
        self.sender_host = '' #邮件服务器 如smtp.qq.com
        self.sender = '' #发件人 如123456@qq.com
        self.sender_password = '' #发件邮箱密码 
        self.receivers = ['123@126.com'] #收件人列表


        self.sentItems = [] #存放已提醒的帖子，避免重复发送

    def getItems(self) :
        """
        返回请求页面[[帖子 url , 帖子标题1，帖子发表日期],[帖子 url , 帖子标题2，帖子发表日期]...]
        """
        try :
            
            user_agent = 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Ubuntu Chromium/38.0.2125.111 Chrome/38.0.2125.111 Safari/537.36'    
            #模拟 post 时设置 'X-Requested-With': 'XMLHttpRequest'
            headers = { 'User-Agent' : user_agent,'X-Requested-With': 'XMLHttpRequest' }

            req = urllib2.Request( url = self.myUrl, headers = headers)
            response = urllib2.urlopen(req)
            page = response.read()
            unicodePage = page.decode("gbk").encode('utf-8')
            pattern = re.compile(r'<td.*?class="title_9.*?><a.*?href="(.*?)">(.*?)</a>.*?<td.*?class="title_10.*?>(.*?)</td>')
            items = pattern.findall(unicodePage)
            return items

        except urllib2.URLError, e :
            print e.code

    def isItemSent(self,item) :
        """
        判断帖子是否已经被发送过了
        """
        for sentItem in self.sentItems:
            if item[1] == sentItem[1] :
                return True
        return False

    def sendMail(self,item):
        
        content = item[1] + ' ' + item[2] + '\n' + self.domain + item[0]
        msg = MIMEText(content,'plain','utf-8')
        msg['Subject'] = Header("Subject",'utf-8')  #attention
        msg['From'] = self.sender
        msg['To'] = ";".join(self.receivers)
        
        try:
            smtpObj = smtplib.SMTP()
            smtpObj.connect(self.sender_host)
            smtpObj.login(self.sender,self.sender_password)
            smtpObj.sendmail(self.sender,self.receivers,msg.as_string())
            smtpObj.close()
            print "Successfully sent email"
            return True
        except smtplib.SMTPException,e :
            print "Error: " + str(e)
            return False

    def start(self):
        while True:
            datetime = time.strftime('%Y-%m-%d',time.localtime(time.time()))
            items = self.getItems();
            for item in items:
                #当帖子中含有 “活动报名贴”，发帖日期为今天，并且没有发过该贴邮件时，发送邮件
                if item[1].find(self.keyword) != -1 and item[2] == datetime and not self.isItemSent(item):
                    if(self.sendMail(item)) :
                        self.sentItems.append(item)
                    else:
                        print "email send error"
            time.sleep(60*self.minute)

if __name__ == '__main__':
    spider = Spider()
    spider.start()
