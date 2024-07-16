import os
import telebot

from controllers.authorization.AuthorizationController import AuthorizationController
from controllers.simple_messages.MessageController import MessageController
from controllers.user.UserController import UserController
from controllers.data_base.DataBaseController import DataBaseController
from working_with_data.user.DataUsers import DataUsers

class Bot:
    def __init__(self, token):
        self.__bot = telebot.TeleBot(token)

        self.__data_users = DataUsers()

        self.__user_controller = UserController(self.__bot, self.__data_users)
        self.__auth_controller = AuthorizationController(self.__bot, self.__data_users)
        self.__message_controller = MessageController(self.__bot)
        self.__data_base_controller = DataBaseController(self.__bot, self.__data_users)

    def __start_controllers(self):
        self.__auth_controller.start_controller()
        self.__message_controller.start_controller()
        self.__user_controller.start_controller()
        self.__data_base_controller.start_controller()

    def run(self):
        self.__start_controllers()
        self.__bot.polling(none_stop=True, interval=0)
