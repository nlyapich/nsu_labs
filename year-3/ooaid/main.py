from bot import Bot

from telegramToken import TELEGRAM_TOKEN

if __name__ == '__main__':
    bot = Bot(TELEGRAM_TOKEN)
    bot.run()
