# Incredicode
Very nice encryption and decryption algorithm to turn garbage into letters.

This is the source code of https://lucascodes.com/encryption/, an algorithm to turn words into gibberish and back. 

**Usage**
Once Ran, the program will want a:
String: This is your data you want to encrypt/decrypt.
Givenkey: This is your key you and your friends can share so that no one else can decrypt it.
Encode: If you answer 'encode' you are encrypting the string with givenkey. If you answer anything else the program will decrypt it.
Xored: XORing makes the encryption more secure and always allows 'onlyletters', but makes the encrypted gibberish string twice as long. If you answer 'xor' it will activate, if you answer anything else, xoring is disabled. (So if you encode, it becomes twice the size of string, or you decode, it becomes half the size)
Onlyletters: If you answer 'onlyletters', this means that your output will contain only lowercase letters, uppercase letters numbers and spaces. This is handy for a bit less cryptic looking gibberish. **Disclaimer:** if you have onlyletters enabled but xoring disabled, your input can also only contain lowercase and uppercase letters (and numbers and spaces). 

The program will the output the encoded/decoded text.

**Disclaimer:** aside from the givenkey, xoring and onlyletters also define settings for your encoding / decoding. This means, if you try to decode an xored onlyletters with the same key but without onlyletters or xored enabled/disabled, you will get a different result! Xoring enabled and onlyletters disabled is recommended.
