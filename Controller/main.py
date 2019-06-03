'''

    Cross-platform controller

    Latest update at Jul.20 2018

    Release version : 3.0j (Maker Faire Tokyo 2018)

    Release notes : Hot fix for pressing shoot too fast

    Coded by Falcon Lee falcon.lee1011@gmail.com

    This code is licensed under the Apache License, Version 2.0
    for more information, please see LICENSE.md


'''
import socket, time, pygame, math

pygame.init()
width, height = pygame.display.Info().current_w, pygame.display.Info().current_h
#Enable or disable UI zoom (Default is 1)
screenxW = width * 10 / 1775
zoom = math.floor(screenxW) / 10

#Config device [sd, hd]
if(zoom >= 0.7):dvc = 'hd'
else:dvc = 'sd'

#size = Window.size
#print("\nScreen resolution " + str(width) + " x " + str(height) + "\nType : " + dvc + " zoom : " + str(zoom))
#if(debugOutput):print("\nWindow size : " + str(size))

#global ui_img, ui_img_U, ui_img_D, ui_img_L, ui_img_R, ui_img_Press
if(dvc == 'hd'):
    ui_img = "./assets/image/ui_1775x1080.jpg"
    ui_img_U = "./assets/image/ui_1775x1080_u.jpg"
    ui_img_D = "./assets/image/ui_1775x1080_d.jpg"
    ui_img_L = "./assets/image/ui_1775x1080_l.jpg"
    ui_img_R = "./assets/image/ui_1775x1080_r.jpg"
    ui_img_Press = "./assets/image/ui_1775x1080_onPress.jpg"
    ui_img_Gameover = "./assets/image/ui_1775x1080_tap.jpg"


elif(dvc == 'sd'):
    ui_img = "./assets/image/ui_1024x554.jpg"
    ui_img_U = "./assets/image/ui_1024x554_u.jpg"
    ui_img_D = "./assets/image/ui_1024x554_d.jpg"
    ui_img_L = "./assets/image/ui_1024x554_l.jpg"
    ui_img_R = "./assets/image/ui_1024x554_r.jpg"
    ui_img_Press = "./assets/image/ui_1024x554_onPress.jpg"
    ui_img_Gameover = "./assets/image/ui_1024x554_tap.jpg"

#Config window
from kivy.config import Config
#Config.set('graphics','resizable',0)
#Config.set('graphics', 'width', width)
#Config.set('graphics', 'height', height)

#For Android **only** libiries
from plyer import vibrator

############# Feature for future update #############
#from kivy.core.audio import SoundLoader
#gunfire = SoundLoader.load('./assets/sound/gun.wav')
#####################################################

#Enable or disable debug output (Default is False)
debugMode = False

#Enable or disable sound effects (Default is False)
#seff = False

#Enable or disable vibration (Default is False)
vib = False

#Enable or disable package sending for debug
beta = True
rst = True

#Enable or disable shot count (Default is True)
shotLim = True

#Enable or disable debug output
debugOutput = False

#Setup address
addr = "192.168.4.1"
addr2 = "192.168.4.3"
port = 80

#Other valuables
st = 0
rrrrr = False #Start new game screen
busy = False

from kivy.app import App
from kivy.uix.label import Label
from kivy.uix.widget import Widget
from kivy.uix.button import Button
from kivy.graphics import Color, Rectangle
from kivy.uix.image import Image
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.gridlayout import GridLayout
from kivy.uix.floatlayout import FloatLayout
from kivy.core.window import Window

if(not beta):
    if(debugOutput):print("\n\n***************\nRunning UI ONLY test\n***************\n\n")

if(beta):s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
if(rst):s2 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

data = "Hello!"
if(beta):s.sendto(data.encode(), (addr, port))
if(rst):s2.sendto(data.encode(), (addr, port))

dx = dy = 0

def postdt2(dat):
    global beta
    if(beta):s2.sendto(dat.encode(), (addr2, port))
    if(debugOutput):print("SEND : Data " + dat + " sent. to 192.168.4.2")

def mmap(omx, omi, nmx, nmi, ov):
    nv = (((ov - omi) * (nmx - nmi)) / (omx - omi)) + nmi
    return nv

def postdt(dat):
    global beta
    if(beta):s.sendto(dat.encode(), (addr, port))
    if(debugOutput):print("SEND : Data " + dat + " sent. to 192.168.4.1")
    #time.sleep(1)

class TI(FloatLayout):
    global dx, dy, aa, addr, port, dataL, data, beta, ui_img, ui_img_U, ui_img_D, ui_img_L, ui_img_R, ui_img_Press, rrrrr, busy

    def __init__(self, **kwargs):
        super(TI, self).__init__(**kwargs)

        self.ui = Image(source=ui_img, size_hint=(1, 1), pos=(0, 0))
        self.add_widget(self.ui)
        self.shotCount = Label(text = 'Shots fired : ' + str(st), size_hint=(.25, .25), pos=(1200 * zoom, 800 * zoom))
        self.add_widget(self.shotCount)

    def shitBtn(self):
        global st, data, beta, rrrrr, busy
        if(not rrrrr):data = "5>"
        if(beta):s.sendto(data.encode(), (addr, port))
        #notification.notify(title="Kivy Notification",message="Plyer Up and Running!",app_name="dick",timeout=10)
        if(vib):vibrator.vibrate(time=0.4)
        #if(seff):gunfire.play()

        #data = "shot>"
        #if(beta):s3.send(data.encode())

        if(debugOutput):print("SEND : Fire command sent.")
        st += 1
        if(debugOutput):print("INFO : Shoot " + str(st) + " times.")
        #labelSt.configure(text="Shoot " + str(st) + " times")
        time.sleep(1)
        busy = True
        if(st == 12 and shotLim):
            if(rst):data = "stop"
            if(rst):postdt2(data)
            if(debugOutput):print("\nINFO limit shots reached.\n")
            #self.ui.source = ui_img_Gameover
            self.ui_govr = Image(source=ui_img_Gameover, size_hint=(1, 1), pos=(0, 0))
            self.add_widget(self.ui_govr)
            rrrrr = True
            #self.remove_widget(self.ui_govr)
            st = 0

    def ff(self):
        if(debugOutput):print("U called ?")
        global dx, dy, st
        posX = mmap(450 * zoom, 0, 180 * zoom, 0, dx)
        posY = mmap(450 * zoom, 0, 180 * zoom, 0, dy)

        if(dx >= 400 * zoom or dx <= 340 * zoom):x = 1
        else:x = 0
        if(dy >= 440 * zoom or dy <= 340 * zoom):y = 1
        else:y = 0

        if(x == 1 or y == 1):
            if(abs(dx - 400 * zoom) < -dy + 540 * zoom):
                postdt("2>")
                self.ui.source = ui_img_D
            elif(abs(dx - 400 * zoom) < dy - 540 * zoom):
                postdt("1>")
                self.ui.source = ui_img_U
            elif(abs(dy - 540 * zoom) < -dx + 400 * zoom):
                postdt("3>")
                self.ui.source = ui_img_L
            elif(abs(dy - 540 * zoom) < dx - 400 * zoom):
                postdt("4>")
                self.ui.source = ui_img_R
        else:
            self.ui.source = ui_img
            postdt("9>")

    def on_touch_down(self, touch):
        global dx, dy, rrrrr, shotLim, busy
        if(rrrrr and shotLim):
            if(rst):data = "rest"
            if(rst):postdt2(data)
            self.remove_widget(self.ui_govr)
            rrrrr = False

        if(debugOutput):print("// DOWN X = " + str(touch.x) + " DOWN Y = " + str(touch.y))
        dx = touch.x
        dy = touch.y
        if(dx <= 800 * zoom):
            if(vib):vibrator.vibrate(0.05)
            self.ff()
            #self.ui.source = "./assets/image/ui_1024x554.jpg"
        elif(dx >= 1300 * zoom and dx <= 1700 * zoom):
            if(dy >= 340 * zoom and dy <= 740 * zoom):
                if(not busy):
                    self.shitBtn()
                    self.ui.source = ui_img_Press
                    self.shotCount.text = "Shots fired : " + str(st)
                    #time.sleep(1)
                if(busy):busy = False

    def on_touch_move(self, touch):
        global dx, dy
        if(debugOutput):print("// MOVE X = " + str(touch.x) + " MOVE Y = " + str(touch.y))
        dx = touch.x
        dy = touch.y
        if(dx <= 800 * zoom):
            if(vib):vibrator.vibrate(0.05)
            self.ui.source = ui_img
            self.ff()

    def on_touch_up(self, touch):
        #vibrator.vibrate(0.1)
        self.ui.source = ui_img
        #self.remove_widget(self.uiuiSt)
        postdt("9>")
        #self.update()

    #if(debugOutput):print("INFO : dx = " + str(dx) + " | dy = " + str(dy))

class dick(App):
    def build(self):
        return TI()

if(__name__ == "__main__"):
    dick().run()
