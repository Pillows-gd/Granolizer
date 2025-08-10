# Granolizer 1.0
Granolizer is a Granular Synthesizer developed for LIM (Laobratorio di Informatica Musicale) with up to 8 different "grain-flows" playing together for an unpredictable and fascinating sound.

Granolizer was developed for LIM (Laboratorio di Informatica Musicale). Download is available at https://audioplugins.lim.di.unimi.it/index.php?p=2

## Documentation (Ita)
<img width="953" height="901" alt="image" src="https://github.com/user-attachments/assets/256a3cc3-600a-497a-bb6d-2c1082e3ddd1" />

### Cos'è?

Granolizer è un sintetizzatore granulare che comprende modulazioni e controllo dell'inviluppo d'ampiezza. Supporta file in input mono e stereo.

Cosa contiene questo pacchetto?

In questo zip trovate questo file (il manuale), il file che indica le differenze tra le varie versioni (Changelog.txt) e i diversi file del plugin, in particolare il plugin è disponibile nei seguenti formati:

<table><tr><th rowspan="2" valign="top"></th><th colspan="2" valign="top">VST2</th><th colspan="2" valign="top">VST3</th><th colspan="2" valign="top">Standalone</th></tr>
<tr><td valign="top">32 Bit</td><td valign="top">64 Bit</td><td valign="top">32 Bit</td><td valign="top">64 Bit</td><td valign="top">32 Bit</td><td valign="top">64 Bit</td></tr>
<tr><td valign="top">Mac OS</td><td valign="top">×</td><td valign="top">√</td><td valign="top">×</td><td valign="top">√</td><td valign="top">×</td><td valign="top">×</td></tr>
<tr><td valign="top">Windows</td><td valign="top">√</td><td valign="top">×</td><td valign="top">×</td><td valign="top">√</td><td valign="top">×</td><td valign="top">√</td></tr>
</table>

### Come si installa?

In base al sistema operativo (Mac OS o Windows), all'architettura (32 o 64 bit), e allo standard che si intende usare (VST2 o VST3) si dovrà copiare il rispettivo file del plugin nella cartella più appropriata:

<table><tr><th valign="top">Tipo di Plugin</th><th valign="top">Tipo di cartella</th><th valign="top">Mac OS Installation Directory</th></tr>
<tr><td rowspan="2" valign="top">VST2</td><td valign="top">Utente</td><td valign="top">~/Library/Audio/Plug-Ins/VST</td></tr>
<tr><td valign="top">Sistema</td><td valign="top">Macintosh HD/Library/Audio/Plug-Ins/VST</td></tr>
<tr><td rowspan="2" valign="top">VST3</td><td valign="top">Utente</td><td valign="top">~/Library/Audio/Plug-Ins/VST3</td></tr>
<tr><td valign="top">Sistema</td><td valign="top">Macintosh HD/Library/Audio/Plug-Ins/VST3</td></tr>
</table>

<table><tr><th valign="top">Tipo di Plugin</th><th valign="top">Tipo di cartella</th><th valign="top">Windows Installation Directory</th></tr>
<tr><td rowspan="4" valign="top">VST2</td><td valign="top">Condivisa</td><td valign="top">C:\Program Files\VstPlugins</td></tr>
<tr><td valign="top">Condivisa</td><td valign="top">C:\Program Files\Steinberg\VstPlugins</td></tr>
<tr><td valign="top">Condivisa</td><td valign="top">C:\Program Files\Common Files\VST2</td></tr>
<tr><td valign="top">Condivisa</td><td valign="top">C:\Program Files\Common Files\Steinberg\VST2</td></tr>
<tr><td valign="top">VST3</td><td valign="top">Condivisa</td><td valign="top">C:\Program Files\Common Files\VST3</td></tr>
</table>

*Nota: Si aggiunga il suffisso " (x86)" alla cartella "Programmi" per i plugin a 32-bit su sistemi a 64-bit.*

### Come si usa?

Per iniziare a generare suoni, caricare dapprima un file .wav o .aiff in input tramite il bottone **Load Sample**. Una volta fatto ciò basterà premere qualsiasi tasto di un MIDI Controller collegato e si otterrà dell'audio in output.

*Nota: è comune che il file audio di input abbia momenti di silenzio. Se non si dovesse sentire alcun suono si provi a cambiare il parametro **Position** o a selezionare una sezione specifica del sample tramite **Start Cut** e/o **End Cut** (vedasi la descrizione dei parametri di seguito).*

A seguire, una descrizione del comportamento dei parametri:

*Sezione di Controllo Generale:*

-**Position**: cambia la posizione di lettura dei vari grani (muovendoli della stessa quantità);

-**Speed**: cambia la velocità di lettura dei vari grani;

-**Grain Phase**: cambia la fase dei vari grani;

-**Position Spread**: disperde i vari grani cambiandone la posizione di lettura (muovendoli di diverse quantità);

-**Grain Number**: cambia il numero di grani attivi;

-**Start Cut**: parametro indicato graficamente dalla "linguetta" sopra la rappresentazione grafica del sample di input. Cambia il punto di inizio di lettura del sample stesso;

-**End Cut**: parametro indicato graficamente dalla "linguetta" sotto la rappresentazione grafica del sample di input. Cambia il punto di fine di lettura del sample stesso;

-**Grain**: cambia il volume di output;

-**Transpose**: applica una trasposizione in semitoni alle note premute da MIDI Controller.

*Sezione di Controllo dell'Inviluppo d'Ampiezza:*

Contiene i parametri di Attack, Decay, Sustain e Release relativi all'inviluppo d'ampiezza attivato alla pressione di un tasto MIDI.

*Sezione di Controllo delle Modulazioni:*

Contiene i parametri di controllo (ampiezza, frequenza e forma d'onda della modulante) delle modulazioni di **Position** e **Grain Phase**.

A seguire, una descrizione del comportamento dei bottoni:

-**Load Sample**: carica il file in input. Formati accettati: .wav, .aiff;

-**Reverse**: inverte i parametri **Start Cut** ed **End Cut**. Questo implica che la direzione di lettura del sample sarà invertita rispetto alla situazione precedente;

-**Undo**: annulla l'ultima modifica dei parametri;

-**Redo**: riesegue l'ultima modifica dei parametri annullata precedentemente;

-**Reset**: reimposta i parametri ai valori di default;

-**Load**: permette di caricare un preset (in formato .xml);

-**Save**: permette di salvare il preset corrente (in formato .xml);

-**Sync Voices**: sincronizza le voci di polifonia;

-**Key-Sync**: attiva il Key-Sync (ogni voce di polifonia riprenderà la lettura del file alla posizione indicata dai parametri appositi nel momento in cui sarà attivata, in modo completamente indipendente dalle altre);

-**x0.5**: imposta i valori di modulazione del parametro **Position** in modo tale da leggere il sample a velocità x0.5;

-**x1.0**: imposta i valori di modulazione del parametro **Position** in modo tale da leggere il sample a velocità x1.0;

-**x2.0**: imposta i valori di modulazione del parametro **Position** in modo tale da leggere il sample a velocità x2.0;

*Nota: I bottoni **x0.5**, **x1.0** e **x2.0** ignorano il valore corrente di **Speed** e **Position Spread**. Per ottenere una vera e propria lettura del file alle varie velocità, impostare **Speed** a 0.00 e **Position Spread** a 0.00000.*

### Limitazioni note

Il plugin è in continua fase di sviluppo, potrebbe essere soggetto a bug. In caso se ne trovi qualcuno, per favore, comunicarlo direttamente allo sviluppatore alla mail: filippo.giabelli@gmail.com, grazie.





![](Aspose.Words.dfca89ac-da01-40f5-8650-2ed0e8b7c35f.002.png)

Questo software è stato sviluppato da Filippo Giabelli al Laboratorio di Informatica Musicale dell'Università degli Studi di Milano. Per info sul laboratorio visitare il sito: [https://www.lim.di.unimi.it](https://www.lim.di.unimi.it/) Per segnalare bug o per altri commenti scrivere a filippo.giabelli@gmail.com
