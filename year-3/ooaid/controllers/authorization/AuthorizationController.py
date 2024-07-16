import telebot
from telebot import types

from controllers.user.UserController import UserController
from controllers.State import State
from controllers.catigories import list_categories
from controllers.locations import list_cities
from working_with_data.user.DataUsers import DataUsers

class AuthorizationController:
    def __init__(self, bot: telebot.TeleBot, data_users: DataUsers):
        self.__bot = bot
        self.__data_users = data_users

    def start_controller(self):
        @self.__bot.message_handler(commands=['start'])
        def start_message(message):
            user_id = message.from_user.id

            self.__data_users.add_new_user(user_id, State.S_AUTHORIZATION.value, '')

            markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
            self.__bot.send_message(user_id, 'Здравствуйте! Выберите город: ' + ' '.join(list_cities), reply_markup=markup)

        @self.__bot.message_handler(func=lambda message: self.__data_users.get_user_data(message.from_user.id).get_state() == State.S_AUTHORIZATION.value)
        def select_location(message):
            user_id = message.from_user.id
            city = message.text.lower()

            if not (city in list_cities):
                self.__bot.send_message(user_id, 'Нет такого города')
                return

            self.__data_users.set_location(user_id, city)
            self.__data_users.set_state(user_id, State.S_CATEGORY.value)

            markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
            self.__bot.send_message(user_id, 'Авторизация завершена! Для получения более подробной информации по использованию бота введите /instruction', reply_markup=markup)
            self.__bot.send_message(user_id, 'Выберите категорию: ' + ' '.join(list_categories), reply_markup=markup)
