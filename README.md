# Blokų grandinė

## Projekto struktūra
```
hash-function/blockchain/src/
├── block/             
    ├── Block.cpp               # Block klasės funkcijų aprašymai
    └── Block.h                 # Block klasės aprašymas ir funkcijų prototipai
├── blockchain/                     
    ├── Blockchain.cpp          # Blockchain klasės funkcijų aprašymai
    └── Blockchain.h            # HashPointer, BlockNode bei Blockchain klasių aprašymai ir funkcijų prototipai
├── hasher/               
    └── hashfunc.hpp            # Maišos funkcijos aprašymas  
├── transaction/               
    ├── Transaction.cpp         # Transaction klasės funkcijų aprašymas
    └── Transaction.h           # Transaction klasės aprašymas ir funkcijų prototipai
├── user/               
    └── User.h                  # User klasės aprašymas
└── main.cpp                    # Vartotojų ir transakcijų generavimas, blokų kasimas
```

## Blokų grandinės realizacija

Sugeneruotos transakcijos saugomos ```blockchain/Blockchain.h``` faile aprašytame ```Blockchain``` klasės kintamajame ```mempool```.
Kol mempool'e yra transakcijų, ```main.cpp``` faile kviečiama ```Blockchain.mineNextBlock()``` funkcija.
```mineNextBlock()``` funkcijoje iš mempool'o atsitiktinai išrenkama iki 100 transakcijų, apskaičiuojamas praeito bloko
hash'as (genesis bloko atveju užpildoma 0) ir sukuriamas naujas blokas. Iškasus bloką, sukuriama ```BlockNode``` instancija,
kurioje saugomas naujas blokas bei praeito bloko ```HashPointer```. ```Blockchain.head``` nurodomas į naują ```BlockNode```.

## Komandinės eilutės sąsaja

### Iškastas blokas
```
(+) BLOCK MINED
Version:      0.1
Timestamp:    1761767903
Nonce:        530
Difficulty:   000
Prev Block Hash:    00000000000000000000000000000000
Transactions Hash:    5f88b25d135775a40670f75951838133
Block Hash:   00033299df87f14a2786a7973ca9e0d7
Tx count:     100
```

### Iškasto bloko transakcijų sąrašas
```
Transactions:
public_key_73 -> public_key_413 amt: 69526 coins / id=0950dcfca9947436305b1de6e3f1a7a1
public_key_471 -> public_key_948 amt: 54093 coins / id=82087a2a820d5265a22cc693b6969bb4
public_key_24 -> public_key_532 amt: 75241 coins / id=bc70151d64ea67f617bd7efa938133fc
public_key_79 -> public_key_258 amt: 39366 coins / id=6ee18697af43f02df08746d208a2187f
...
```

### Blokų grandinė
```
...
Block no. 1:
  Block Hash:   000c70fc3fe7a649a671ed7cda323257
  Prev Block Hash:    00033299df87f14a2786a7973ca9e0d7
  Nonce:        4206
  Tx Count:     100
  HashPointer.prev_block_ptr:  0x1a1b4b610b0
  HashPointer.prev_block_hash: 00033299df87f14a2786a7973ca9e0d7

Block no. 0:
  Block Hash:   00033299df87f14a2786a7973ca9e0d7
  Prev Block Hash:    00000000000000000000000000000000
  Nonce:        530
  Tx Count:     100
  HashPointer.prev_block_ptr:  nullptr
  HashPointer.prev_block_hash: 00000000000000000000000000000000
  ```
