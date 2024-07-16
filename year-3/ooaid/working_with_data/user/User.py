class User:
    def __init__(self, user_id, state, city, category):
        self.__user_id = user_id
        self.__state = state
        self.__city = city
        self.__category = category

    def get_user_id(self):
        return self.__user_id

    def get_location(self):
        return self.__city

    def get_category(self):
        return self.__category

    def get_state(self):
        return self.__state

    def set_location(self, location):
        self.__city = location

    def set_category(self, category):
        self.__category = category

    def set_state(self, state):
        self.__state = state
