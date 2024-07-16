import json

class DataBaseProducts:
    def __init__(self):
        self.__data_products_file = 'result.json'

    def find_product(self, product, user):
        user_category = user.get_category()
        user_product = product.lower().split()
        user_location = user.get_location()

        good_products = []
        middle_products = []

        with open(self.__data_products_file, "r") as read_file:
            data = json.load(read_file)
            for shop in data['shops']:
                for location in shop['locations']:
                    if not (location['locationName'] == user_location):
                        continue
                    for category in location['categories']:
                        if not (category['categoryName'] == user_category):
                            continue

                        for product in category['products']:
                            cur_product = product['productName'].lower().split()
                            count = 0
                            for elem_product in user_product:
                                if (elem_product in cur_product):
                                    count += 1
                            coef = count / len(cur_product)
                            if (coef > 0.7):
                                good_products.append(product)
                            elif (coef > 0.4):
                                middle_products.append(product)

        return [good_products, middle_products]
