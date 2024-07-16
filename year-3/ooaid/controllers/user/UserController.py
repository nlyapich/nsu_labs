import telebot
from telebot import types

from controllers.State import State
from controllers.catigories import list_categories
from controllers.locations import list_cities
from working_with_data.user.DataUsers import DataUsers

class UserController:
    def __init__(self, bot: telebot.TeleBot, data_users: DataUsers):
        self.__bot = bot
        self.__data_users = data_users

    def start_controller(self):
        @self.__bot.message_handler(func=lambda message: self.__data_users.get_user_data(message.from_user.id).get_state() == State.S_CATEGORY.value)
        def select_category(message):
            user_id = message.from_user.id
            category = message.text.lower()

            if not (category in list_categories):
                self.__bot.send_message(user_id, 'Нет такой категории. Введите другую.')
                return

            self.__data_users.update_category(user_id, category)
            self.__data_users.set_state(user_id, State.S_PRODUCT.value)

            markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
            self.__bot.send_message(user_id, 'Категория выбрана! Введите название товара.', reply_markup=markup)

        @self.__bot.message_handler(commands=['update_category'])
        def start_update_category(message):
            user_id = message.from_user.id
            is_check = self.__data_users.check_user(user_id)
            if not (is_check):
                markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
                self.__bot.send_message(user_id, 'Вы еще не авторизовались!', reply_markup=markup)
                return

            self.__data_users.set_state(user_id, State.S_UPDATE_CATEGORY.value)

            markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
            self.__bot.send_message(user_id, 'Введите новую категорию: ' + ' '.join(list_categories), reply_markup=markup)

        @self.__bot.message_handler(func=lambda message: self.__data_users.get_user_data(message.from_user.id).get_state() == State.S_UPDATE_CATEGORY.value)
        def update_category(message):
            user_id = message.from_user.id
            category = message.text.lower()

            if not (category in list_categories):
                self.__bot.send_message(user_id, 'Нет такой категории. Введите другую.')
                return

            self.__data_users.update_category(user_id, category)
            self.__data_users.set_state(user_id, State.S_PRODUCT.value)

            markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
            self.__bot.send_message(user_id, 'Категория обновлена! Введите название товара.', reply_markup=markup)

        @self.__bot.message_handler(commands=['change_location'])
        def start_change_location(message):
            user_id = message.from_user.id
            self.__data_users.set_state(user_id, State.S_CHANGE_LOCATION.value)

            markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
            self.__bot.send_message(user_id, 'Введите новую локацию: ' + ' '.join(list_cities), reply_markup=markup)

        @self.__bot.message_handler(func=lambda message: self.__data_users.get_user_data(message.from_user.id).get_state() == State.S_CHANGE_LOCATION.value)
        def change_location(message):
            user_id = message.from_user.id
            city = message.text.lower()

            if not (city in list_cities):
                self.__bot.send_message(user_id, 'Нет такого города')
                return

            self.__data_users.set_location(user_id, city)
            self.__data_users.set_state(user_id, State.S_PRODUCT.value)

            markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
            self.__bot.send_message(user_id, 'Локация изменена', reply_markup=markup)
