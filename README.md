# Connected-CatDoor: NEKODO
Connected catdoor project, realized during IoT course at INSA Rennes
Authors: Nathan BALIX, Matthieu DARFAY, Zoé LEVREL, Mathieu RISY

## Origin of the project
Our teacher gave us this text:

*"When you have cats at home, you want to be able to manage when they go out and to protect your house from your neighbours' cats intrusion through your mechanical cat door. The Connected Cat Door solves these different issues by letting you manage your cats authorizations to go in/out. Thus, you can define that a specific cat has the right to go out but can't come inside, while another one is free to go wherever it wants."*

From this, we defined 3 main features for this project:
- Managing the cats' permissions
- Reading the cats' tag (a tag identifies a tag and is situated in the cat's neck)
- Providing a mechanism to (un)lock the cat door

The name *"NEKODO"* comes from two japanese words: *neko* (cat) and *doa* (door).

## Permissions management
### Web GUI
In order to be able to modify each cat's permissions to go in/out of the house, the user can
connect to a web server. This server sends web pages, from which you can add or remove a cat, and manage all the permissions. It's also possible to know live if a cat is inside or outside the house (this information is saved whenever a cat walks through the cat door).

The M5Stack enables to handle Wifi via the ESP32 module. It can be set as an Access Point (AP), and thus enables web clients to connect to it. The M5Stack is then used as a server. All the pages of the GUI are stored in the M5Stack and sent directly to the users connected to the Wifi hotspot that it exposes.

Another possible solution was to connect the M5Stack to a Wifi hotspot, which allows to communicate with a distant web server (to send cat information for example). This solution wasn’t kept but is still implementable.

### Data back-up
The M5Stack can save cats’ information thanks to the `preferences`. They are updated for each modification, deletion or addition in the cat dababase. It avoids losing all cats’ permissions if the connected cat door is shut down. Cats’ information is only saved in the “master” M5Stack.

### Communication between both M5Stack
In order to handle the cat door (un)locking when a cat arrives, both M5Stack need to communicate with each other, because the “master” M5Stack is the only one to store the cats' permissions.

When a NFC chip is read, the “slave” M5Stack sends the ID to the “master” one, which returns the cat's associated permission. Communication is handled with serial links (both devices are connected to the other one).

## Cats’ chip reading: NFC
The cats’ RFID chip reading was simulated with NFC tags. With a Seeedstudio GROVE NFC PN532 antenna (connected in I2C) and its libraries, the M5Stack can get the NFC tag information it needs. The connected cat door only needs the tag ID.

When the antenna detects an NFC tag, the ID can be added to the database. Other information will be provided by connection to the web server (permissions, name, etc.). This reading is also used when a cat wants to go in/out, to unlock the door if it is authorized.

## Cat door (un)lock system: servomotors
The lock system is managed by two servomotors, each one handling one gateway side (go in or out). The servomotors we use don’t work with I2C but with PWM whereas the M5Stack uses I2C. We have to use the LED functions in order to simulate a PWM running (`ledcSetup` and `ledcAttachPin`).

When a cat door side needs to be unlocked, the associated servomotor makes a 90° rotation, which lowers the small stick locking the door. Then, there is a small waiting time to allow the cat to cross the door. Finally, the servomotor locks the door once again.

It is also possible to change the lock position by pressing the A button on the M5Stack.
