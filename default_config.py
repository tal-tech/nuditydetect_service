# -*- coding: utf-8 -*-
import os
from enum import IntEnum
import time

class EureakaEnv(IntEnum):
    dev = 1
    test = 2
    ali_product = 3

def getEnvValue():
    for i in range(3):
        if "CURRENT_ENV" in os.environ:
            break
        else:
            time.sleep(1)
            continue
    assert "CURRENT_ENV" in os.environ, "EnvError: CURRENT_ENV is not found."
    env = os.environ['CURRENT_ENV']
    assert env in ['TEST', 'PRE', 'PROD'], "KeyError: %s is not in ['TEST', 'PRE', 'PROD']".format(env)
    if env == 'TEST':
        filename = 'config_test.ini'
    elif env == 'PRE':
        filename = 'config_pre.ini'
    elif env == 'PROD':
        filename = 'config_product.ini'
    #print("Successful load config file: %s" % env)
    return filename


DATAWORK_NAMESPACE = str(os.environ.get("DATAWOEK_NAMESPACE")) or "datawork-aliyun"
CONFIG_FILE_DIR_PATH = os.path.dirname(os.path.realpath(__file__))
CONFIG_FILE_PATH = os.path.join(CONFIG_FILE_DIR_PATH, './config/'+getEnvValue())
#print("Config file path: %s" % CONFIG_FILE_PATH)
CONFIG_SECTION_LOCAL_SERVER = 'local_server'
CONFIG_OPTION_SERVER_PORT = 'port'
CONFIG_SECTION_EUREKA_SERVER = 'eureka_server'
CONFIG_OPTION_HOST = 'host'
CONFIG_SECTION_URL_TRANS = 'url_trans'
CONFIG_SECTION_MQ_SERVER = 'mq_server'
CONFIG_SECTION_APOLLO_SERVER = 'apollo_server'
CONFIG_OPTION_APPLICATION = "application"
SERVER_PORT = 8761 

Curr_EureakaEnv = EureakaEnv.ali_product

HTTP_OK = 200


class TaskInfo:
    def __init__(self):
        self._task_id = str()
        self._url = str()
        self._base64 = str()
        self._request_id = str()
        self._app_key = str()
        self._api_id = str()
        self._api_type = str()
        self._duration = int()
        self._request_time = int()
        self._response_time = int()
        self._request_data = str()
        self._code = int()
        self._msg = str()
        self._err_code = int()
        self._err_msg = str()

    @property
    def task_id(self):
        return self._task_id

    @task_id.setter
    def task_id(self, val):
        self._task_id = val

    @property
    def url(self):
        return self._url

    @url.setter
    def url(self, val):
        self._url = val

    @property
    def base64(self):
        return self._base64

    @base64.setter
    def base64(self, val):
        self._base64 = val

    @property
    def request_id(self):
        return self._request_id

    @request_id.setter
    def request_id(self, val):
        self._request_id = val
    
    @property
    def app_key(self):
        return self._app_key
    
    @app_key.setter
    def app_key(self, val):
        self._app_key = val

    @property
    def app_id(self):
        return self._api_id
    
    @app_id.setter
    def app_id(self, val):
        self._api_id = val
    
    @property
    def app_type(self):
        return self._api_type
    
    @app_type.setter
    def app_type(self, val):
        self._api_type = val

    @property
    def duration(self):
        return self._duration

    @duration.setter
    def duration(self, val):
        self._duration = val

    @property
    def request_time(self):
        return self._request_time

    @request_time.setter
    def request_time(self, val):
        self._request_time = val

    @property
    def response_time(self):
        return self._response_time

    @response_time.setter
    def response_time(self, val):
        self._response_time = val

    @property
    def request_data(self):
        return self._request_data

    @request_data.setter
    def request_data(self, val):
        self._request_data = val
    
    @property
    def code(self):
        return self._code

    @code.setter
    def code(self, val):
        self._code = val
    
    @property
    def msg(self):
        return self._msg

    @msg.setter
    def msg(self, val):
        self._msg = val

    @property
    def err_code(self):
        return self._err_code

    @err_code.setter
    def err_code(self, val):
        self._err_code = val
    
    @property
    def err_msg(self):
        return self._err_msg

    @err_msg.setter
    def err_msg(self, val):
        self._err_msg = val
