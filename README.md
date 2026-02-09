# English Version

## Prerequisites

The execution of the benchmark suite requires the availability of the following software tools:

* **bc**: arbitrary precision calculator language used for statistical result processing
  ([https://www.gnu.org/software/bc/](https://www.gnu.org/software/bc/))
* **gnuplot**: plotting utility for results visualization
  ([http://www.gnuplot.info/](http://www.gnuplot.info/))

## Execution

The complete test campaign can be launched by executing the following command:

```sh
sh test.sh
```

## Test Methodology

The benchmarks perform a total number of operations equal to:

* 1000
* 10000
* 100000
* 1000000

For each configuration, different **write/read ratios** are considered, ranging from **95%/5%** to **30%/70%**.

Each experimental configuration is repeated **1,000 times** in order to estimate:

* the **average execution time**;
* the **95% confidence interval**.

The tests are executed both:

* with cache flush enabled;
* without cache flush.

When enabled, the cache flush operation is performed:

* before write and read operations;
* before restore operations.

## Experimental Configurations

### MVM_GRID_CKPT_BS

For **MVM_GRID_CKPT_BS**, experiments are repeated by varying:

* grid size:

  * 8 bytes
  * 16 bytes
  * 32 bytes
  * 64 bytes
* total memory size:

  * 1 MB
  * 2 MB
  * 4 MB

The reference implementation is available at:

* [https://github.com/mazzucchi-andrea/MVM/tree/GRID_CKPT](https://github.com/mazzucchi-andrea/MVM/tree/GRID_CKPT)

### MVM_CHUNK_SET

For **MVM_CHUNK_SET**, experiments are conducted considering:

* eight chunk sizes:

  * 32 bytes
  * 64 bytes
  * 128 bytes
  * 256 bytes
  * 512 bytes
  * 1024 bytes
  * 2048 bytes
  * 4096 bytes
* three total memory sizes:

  * 1 MB
  * 2 MB
  * 4 MB

## Assumptions and Implementation Details

* In both configurations, **no memory save operation is performed prior to the write phase**.
* It is assumed that the **checkpoint setup operation** has already been executed; during the tests, only the **bitmap reset** is performed.
* In the **MVM_CHUNK_SET** configuration, the memory area is considered **preallocated** and divided into chunks of size *C*.
* Each write operation on a chunk sets the corresponding bit in the bitmap, even if the bit is already set.

## Experimental Objective

The goal of these experiments is to analyze and compare the performance impact of different checkpoint management strategies, evaluating their behavior with respect to:

* number of operations;
* write/read ratio;
* memory size;
* granularity of the modification tracking mechanism.

## Script Execution Flow

The `test.sh` script automates the following steps:

1. execution of benchmarks for **MVM_GRID_CKPT_BS**;
2. execution of benchmarks for **MVM_CHUNK_SET**;
3. generation of result plots using **gnuplot**.

---

# Versione Italiana

## Prerequisiti

L’esecuzione della suite di benchmark richiede la disponibilità dei seguenti strumenti software:

* **bc**: linguaggio di calcolo arbitrario utilizzato per l’elaborazione dei risultati statistici
  ([https://www.gnu.org/software/bc/](https://www.gnu.org/software/bc/))
* **gnuplot**: strumento per la generazione dei grafici
  ([http://www.gnuplot.info/](http://www.gnuplot.info/))

## Modalità di esecuzione

L’intera campagna di test può essere avviata mediante l’esecuzione del seguente comando:

```sh
sh test.sh
```

## Metodologia di test

I benchmark eseguono un numero complessivo di operazioni pari a:

* 10³
* 10⁴
* 10⁵
* 10⁶

Per ciascun valore, vengono considerati diversi rapporti **scritture/letture**, variabili da **95%/5%** fino a **30%/70%**.

Ogni configurazione sperimentale viene ripetuta **1.000 volte**, al fine di stimare:

* il **tempo medio di esecuzione**;
* l’**intervallo di confidenza al 95%**.

I test vengono condotti sia:

* in presenza di un’operazione di *cache flush*;
* in assenza di *cache flush*.

Quando abilitata, l’operazione di *cache flush* viene eseguita:

* prima delle operazioni di scrittura e lettura;
* prima delle operazioni di *restore*.

## Configurazioni sperimentali

### MVM_GRID_CKPT_BS

Nel caso **MVM_GRID_CKPT_BS**, l’esperimento viene ripetuto variando:

* la dimensione della griglia:

  * 8 byte
  * 16 byte
  * 32 byte
  * 64 byte
* la dimensione complessiva della memoria:

  * 1 MB
  * 2 MB
  * 4 MB

Il codice di riferimento è disponibile al seguente indirizzo:

* [https://github.com/mazzucchi-andrea/MVM/tree/GRID_CKPT](https://github.com/mazzucchi-andrea/MVM/tree/GRID_CKPT)

### MVM_CHUNK_SET

Nel caso **MVM_CHUNK_SET**, i test vengono eseguiti considerando:

* otto diverse dimensioni dei chunk:

  * 32 byte
  * 64 byte
  * 128 byte
  * 256 byte
  * 512 byte
  * 1024 byte
  * 2048 byte
  * 4096 byte
* tre dimensioni complessive della memoria:

  * 1 MB
  * 2 MB
  * 4 MB

## Assunzioni e dettagli implementativi

* In entrambe le configurazioni **non viene effettuata alcuna operazione di salvataggio della memoria prima della fase di scrittura**.
* Si assume che l’operazione di **impostazione del checkpoint** sia stata già eseguita; durante i test viene esclusivamente effettuato il **reset della bitmap**.
* Nel caso **MVM_CHUNK_SET**, l’area di memoria è considerata **preallocata** e suddivisa in chunk di dimensione *C*.
* Ogni operazione di scrittura su un chunk determina l’impostazione del bit corrispondente nella bitmap, anche qualora tale bit risulti già impostato.

## Obiettivo sperimentale

L’obiettivo dei test è analizzare e confrontare l’impatto prestazionale di differenti strategie di gestione dei checkpoint, valutandone il comportamento al variare di:

* numero di operazioni;
* rapporto tra scritture e letture;
* dimensione della memoria;
* granularità del meccanismo di tracciamento delle modifiche.

## Flusso di esecuzione dello script

Lo script `test.sh` automatizza le seguenti fasi:

1. esecuzione dei benchmark per **MVM_GRID_CKPT_BS**;
2. esecuzione dei benchmark per **MVM_CHUNK_SET**;
3. generazione dei grafici dei risultati mediante **gnuplot**.
