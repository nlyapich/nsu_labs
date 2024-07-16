import json

from working_with_data.user.User import User

class DataUsers:
    def __init__(self):
        self.__data_users = []
        self.__data_users_file = 'data_users.json'

    def __find_user(self, user_id):
        for i in range(len(self.__data_users)):
            if (self.__data_users[i].get_user_id() == user_id):
                return i
        return -1

    def __load_user(self, user_id):
        with open(self.__data_users_file, 'r') as f:
            data = json.load(f)

            for user in data['users']:
                cur_user_id = user['user_id']
                if not (user_id == cur_user_id):
                    continue

                state = user['state']
                city = user['city']
                category = user['category']

                self.__data_users.append(User(user_id, state, city, category))
                break

    def check_user(self, user_id):
        pos = self.__find_user(user_id)
        if not (pos == -1):
            return True

        self.__load_user(user_id)
        pos = self.__find_user(user_id)
        if not (pos == -1):
            return True

        return False

    def add_new_user(self, user_id, state, city):
        data = {}
        new_data = {'user_id' : user_id, 'state' : state, 'city' : city, 'category' : ''}
        with open(self.__data_users_file, 'r') as f:
            data = json.load(f)
            data['users'].append(new_data)
        with open(self.__data_users_file, 'w') as outfile:
            json.dump(data, outfile, ensure_ascii=False, indent=2)

        self.__data_users.append(User(user_id, 0, city, ''))

    def update_category(self, user_id, category):
        data = {}
        with open(self.__data_users_file, 'r') as f:
            data = json.load(f)

            for user in data['users']:
                cur_user_id = user['user_id']
                if not (user_id == cur_user_id):
                    continue

                user['category'] = category
                break

        with open(self.__data_users_file, 'w') as f:
            json.dump(data, f, ensure_ascii=False, indent=2)

        pos = self.__find_user(user_id)
        if (pos == -1):
            self.__load_user(user_id)
            return

        self.__data_users[pos].set_category(category)

    def get_user_data(self, user_id):
        pos = self.__find_user(user_id)
        if (pos == -1):
            self.__load_user(user_id)
            return self.__data_users[len(self.__data_users) - 1]

        return self.__data_users[pos]

    def set_location(self, user_id, city):
        data = {}
        with open(self.__data_users_file, 'r') as f:
            data = json.load(f)

            for user in data['users']:
                cur_user_id = user['user_id']
                if not (user_id == cur_user_id):
                    continue

                user['city'] = city
                break

        with open(self.__data_users_file, 'w') as f:
            json.dump(data, f, ensure_ascii=False, indent=2)

        pos = self.__find_user(user_id)
        if (pos == -1):
            self.__load_user(user_id)
            return

        self.__data_users[pos].set_location(city)

    def set_state(self, user_id, state):
        data = {}
        with open(self.__data_users_file, 'r') as f:
            data = json.load(f)

            for user in data['users']:
                cur_user_id = user['user_id']
                if not (user_id == cur_user_id):
                    continue

                user['state'] = state
                break

        with open(self.__data_users_file, 'w') as f:
            json.dump(data, f, ensure_ascii=False, indent=2)

        pos = self.__find_user(user_id)
        if (pos == -1):
            self.__load_user(user_id)
            return

        self.__data_users[pos].set_state(state)
