from selenium import webdriver
from selenium.webdriver.common.by import By
import time

# Initialize the Chrome WebDriver
driver = webdriver.Chrome()
driver.implicitly_wait(10)  # Wait for elements to load

# Open WhatsApp Web
driver.get('https://web.whatsapp.com')

# Wait for the user to scan the QR code and log in
print("Scan the QR code and press Enter after login...")
input()

# Variable to keep track of the last message sent
last_message = ""

chat_name = "אימוש"  # The name of the chat we're looking for


def find_chat_by_name():
    """
    This function checks for the specified chat by its name and checks if there are unread messages.
    """
    try:
        # Find the chat element by the title (use the name of the chat in Hebrew)
        chat = driver.find_element(By.XPATH, f"//span[@title='{chat_name}']")
        print(f"Chat '{chat_name}' found.")
        chat.click()  # Open the chat

        try:
            unread_divider = driver.find_element(By.XPATH, "//div[@id='main']//span[@class='_agtk']")
            if unread_divider:
                print("Unread messages found in the chat!")
                return True
        except Exception as e:
            print(f"No unread message divider found: {e}")
            return False
    except Exception as e:
        print(f"Chat '{chat_name}' not found or no unread messages: {e}")
        return False


def get_last_message():
    """
    This function gets the unique ID or timestamp of the last message.
    """
    try:
        # Find the latest message bubble
        message_bubbles = driver.find_elements(By.XPATH, ".//span[contains(@class, 'selectable-text')]")
        if message_bubbles:
            return message_bubbles[-1].text
    except Exception as e:
        print(f"Error fetching last message ID: {e}")
        return None


def send_reply():
    """
    Sends a predefined reply to the chat.
    """
    try:
        # Find the message input box
        input_box = driver.find_element(By.XPATH, "//footer//div[@contenteditable='true' and @role='textbox']")

        # Type a reply (you can customize this message)
        reply_message = "אל תדאגי הכל טוב אימוש"
        input_box.send_keys(reply_message)

        # Press the 'Send' button
        send_button = driver.find_element(By.XPATH, "//*[@id='main']//button[@aria-label='Send']")
        send_button.click()

        return reply_message

        print(f"Sent reply: {reply_message}")
    except Exception as e:
        print(f"Error sending reply: {e}")

# Main loop to check for new/unread messages and send a reply
while True:
    try:
        # Check for the specified chat and whether it has unread messages
        unread_found = find_chat_by_name()

        if unread_found:
            print(f"Entering chat '{chat_name}', waiting for messages to load...")  # Debugging log
            time.sleep(3)  # Wait for the chat to load

            # Get the last message in the chat
            new_message = get_last_message()
            if new_message == last_message:
                print("No new messages detected.")
                continue
            else:
                print(f"New message detected in chat : '{new_message}'.")
                last_message = send_reply()

        else:
            print(f"No unread messages in '{chat_name}' at the moment.")  # Debugging log

        # Wait for 10 seconds before checking for new messages or unread chats again
        time.sleep(10)

    except Exception as e:
        print("An error occurred:", e)
        break

# Close the browser when done
driver.quit()
