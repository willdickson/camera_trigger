from __future__ import print_function
import serial
import time
import json

class CameraTrigger(serial.Serial):

    BAUDRATE = 115200

    def __init__(self,port,timeout=2.0,connect_sleep_dt=2.0):
        param = {'baudrate': self.BAUDRATE, 'timeout': timeout}
        super(CameraTrigger,self).__init__(port,**param)
        time.sleep(connect_sleep_dt)


    def start(self):
        cmd_dict = {'cmd':'start'}
        self.send_cmd(cmd_dict)


    def stop(self):
        cmd_dict = {'cmd':'stop'}
        self.send_cmd(cmd_dict)


    def set_freq(self,freq):
        """ set pulse frequency in Hz """
        cmd_dict = {'cmd':'set-freq','freq':freq}
        self.send_cmd(cmd_dict)


    def get_freq(self):
        cmd_dict = {'cmd':'get-freq'}
        rsp_dict= self.send_cmd(cmd_dict)
        return rsp_dict['freq']


    def set_width(self,width):
        """ set pulse width in us """
        cmd_dict = {'cmd':'set-width','width':width}
        self.send_cmd(cmd_dict)


    def get_width(self):
        cmd_dict = {'cmd':'get-width'}
        rsp_dict= self.send_cmd(cmd_dict)
        return rsp_dict['width']


    def send_cmd(self,cmd_dict):
        cmd_json = json.dumps(cmd_dict)
        self.write('{}\n'.format(cmd_json))
        rsp_json = self.readline()
        rsp_dict = json.loads(rsp_json)
        self.check_ok(rsp_dict)
        return rsp_dict


    def check_ok(self,rsp):
        ok = rsp['ok']
        if not ok:
            raise RuntimeError('response not ok!, msg = {}'.format(rsp['msg']))



