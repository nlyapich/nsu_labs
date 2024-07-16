class SimpleMessage:
    @staticmethod
    def get_text_rules():
        return """Правила по использованию бота:
            1)  Не использовать бота в коммерческих целях
            2)  Запрещается намеренно перегружать бота запросами"""

    @staticmethod
    def get_text_instruction():
        return """Команды:
            Обновить категорию - /update_category
            Обновить локацию - /change_location
            Правила пользования - /rules
            Контакты для связи - /contacts_communication
            Поддержать разработчиков - /donate"""

    @staticmethod
    def get_text_donate():
        return """Донат:
            9999 9999 9999 9999"""

    @staticmethod
    def get_text_contacts_communication():
        return """Контакты для связи:
            Telegram: @telegram_account
            Mail: mail@gmail.com"""
