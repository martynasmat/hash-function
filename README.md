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

- Paleidus programą, sugeneruojama 1000 vartotojų ir 10000 transakcijų
- Išvedama visų iškastų blokų informacija bei kiekvieno bloko transakcijų sąrašas
- Atvaizduojama visa blokų grandinė, nuo genesis iki paskutinio bloko

- Iškasus visus blokus, komandinės eilutės sąsajos pagalba galima sužinoti:
  - blokų skaičių grandinėje su ```count```
    ```
    >count
     
    Blocks: 100
    ```
  - paskutinio bloko grandinėje informaciją su ```tip```
    ```
    >tip

    Block 99
    Hash:        000af8da6017a3ccbeea9a2481bd493d
    Prev Hash:   000b90d629605342c9b424c1dc5a2e3f
    Version:     0.1
    Timestamp:   1761772829
    Nonce:       8040
    Tx count:    100
    Transactions:
    public_key_651 -> public_key_865 : 2335 | tx_id=1494b6cedb6732e240d8187f9e2e4c20
    public_key_247 -> public_key_823 : 81983 | tx_id=2ff8d1ca52918e0bd79f7b2d540574c5
    ...
    ```
  - n-tojo bloko grandinėje informaciją su ```show N```
    ```
    >show 4

    Block 4
    Hash:        000c837173c2fc19c43c8ad229abc384
    Prev Hash:   000fc8fcd41d9b6fa733def3e527b11f
    Version:     0.1
    Timestamp:   1761772822
    Nonce:       2334
    Tx count:    100
    ```
  - n-tojo bloko grandinėje informaciją ir transakcijų sąrašą su ```showfull N```
  ```
  >showfull 4

  Block 4
  Hash:        000c837173c2fc19c43c8ad229abc384
  Prev Hash:   000fc8fcd41d9b6fa733def3e527b11f
  Version:     0.1
  Timestamp:   1761772822
  Nonce:       2334
  Tx count:    100
  Transactions:
  public_key_931 -> public_key_249 : 86704 | tx_id=b6b1122446be9e0b7a8f762aa26cb1f4
  public_key_791 -> public_key_748 : 44594 | tx_id=8bc3d859bdb2a67bdb9ae591785e0c51
  ...
  ```

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
