import telebot
from telebot import types
from controllers.simple_messages.SimpleMessage import SimpleMessage

class MessageController:
    def __init__(self, bot: telebot.TeleBot):
        self.__bot = bot
        self.__commands = {'rules' : SimpleMessage.get_text_rules(),
                    'instruction' : SimpleMessage.get_text_instruction(),
                    'donate' : SimpleMessage.get_text_donate(),
                    'contacts_communication' : SimpleMessage.get_text_contacts_communication()
                    }

    def start_controller(self):
        @self.__bot.message_handler(commands=['rules'])
        def get_rules(message):
            text = self.__commands.get('rules')
            markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
            self.__bot.send_message(message.from_user.id, text, reply_markup=markup)

        @self.__bot.message_handler(commands=['instruction'])
        def get_instruction(message):
            text = self.__commands.get('instruction')
            markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
            self.__bot.send_message(message.from_user.id, text, reply_markup=markup)

        @self.__bot.message_handler(commands=['donate'])
        def get_donate(message):
            text = self.__commands.get('donate')
            markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
            self.__bot.send_message(message.from_user.id, text, reply_markup=markup)

        @self.__bot.message_handler(commands=['contacts_communication'])
        def get_contacts(message):
            text = self.__commands.get('contacts_communication')
            markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
            self.__bot.send_message(message.from_user.id, text, reply_markup=markup)
