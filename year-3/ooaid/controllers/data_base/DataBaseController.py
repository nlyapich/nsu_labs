import json
import telebot
from telebot import types

from controllers.user.UserController import UserController
from controllers.State import State
from working_with_data.user.DataUsers import DataUsers
from working_with_data.product.DataBaseProducts import DataBaseProducts

class DataBaseController:
    def __init__(self, bot: telebot.TeleBot, data_users: DataUsers):
        self.__bot = bot
        self.__data_users = data_users
        self.__data_base_products = DataBaseProducts()

    def products2str(self, products):
        string_products = ''
        for product in products:
            string_products += '\n' + product['productName'] + ', price = ' + str(product['productPrice']) + ', link = ' + product['productLink']

        return string_products

    def start_controller(self):
        @self.__bot.message_handler(func=lambda message: self.__data_users.get_user_data(message.from_user.id).get_state() == State.S_PRODUCT.value)
        def find_product(message):
            user_id = message.from_user.id
            user = self.__data_users.get_user_data(user_id)

            result_products = self.__data_base_products.find_product(message.text, user)

            good_products = result_products[0]
            middle_products = result_products[1]

            def custom_key(product):
                return product['productPrice']

            good_products.sort(key=custom_key)
            middle_products.sort(key=custom_key)

            markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
            if good_products:
                self.__bot.send_message(user_id, self.products2str(good_products[:15]), reply_markup=markup)
            if middle_products:
                self.__bot.send_message(user_id, self.products2str(middle_products[:15]), reply_markup=markup)

            if not (good_products or middle_products):
                self.__bot.send_message(user_id, 'Такой товар не найден. Попробуйте уточнить модель.', reply_markup=markup)
