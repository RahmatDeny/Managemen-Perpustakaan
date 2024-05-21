#include <iostream>
using namespace std;

struct Buku
{
    string judul;
    string pengarang;
    int tahunTerbit;
    int stok;
    string rak;
    int nomorKotak;
    Buku *kiri;
    Buku *kanan;

    Buku(string j, string p, int t, int s, string r, int nomor)
    {
        judul = j;
        pengarang = p;
        tahunTerbit = t;
        stok = s;
        rak = r;
        nomorKotak = nomor;
        kiri = kanan = nullptr;
    }
};

struct Peminjaman
{
    string namaPeminjam;
    string judulBuku;
    bool dipinjam;

    Peminjaman()
    {
        namaPeminjam = "";
        judulBuku = "";
        dipinjam = false;
    }

    Peminjaman(string nama, string judul)
    {
        namaPeminjam = nama;
        judulBuku = judul;
        dipinjam = true;
    }
};
struct Node {
    Peminjaman peminjaman;
    Node* next;

    Node(Peminjaman p) : peminjaman(p), next(nullptr) {}
};
struct Anggota
{
    string nama;
    string nomorKontak;
    string email;
    string password;
};

class HashTable {
private:
    static const int TABLE_SIZE = 100;
    Node* table[TABLE_SIZE];

    int hashFunction(string key) {
        int hash = 0;
        for (char ch : key) {
            hash = (hash + int(ch)) % TABLE_SIZE;
        }
        return hash;
    }

public:
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            table[i] = nullptr;
        }
    }

    void addPeminjaman(Peminjaman p) {
        int index = hashFunction(p.namaPeminjam + p.judulBuku);
        Node* newNode = new Node(p);
        newNode->next = table[index];
        table[index] = newNode;
    }

    bool ubahStatusPeminjaman(string namaPeminjam, string judulBuku) {
        int index = hashFunction(namaPeminjam + judulBuku);
        Node* current = table[index];

        while (current != nullptr) {
            if (current->peminjaman.namaPeminjam == namaPeminjam && current->peminjaman.judulBuku == judulBuku) {
                current->peminjaman.dipinjam = false;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void printPeminjaman() {
        cout << "=====================================\n";
        cout << "Detail Peminjaman Buku\n";
        cout << "=====================================\n";
        cout << "|--------------------------------------------------------------|\n";
        cout << "| Nama Peminjam      | Judul Buku        | Status              |\n";
        
        for (int i = 0; i < TABLE_SIZE; ++i) {
            Node* current = table[i];
            while (current != nullptr) {
                
                cout << "| " << padRight(current->peminjaman.namaPeminjam, 18) << " | ";
                cout << padRight(current->peminjaman.judulBuku, 18) << " | ";
                cout << padRight(current->peminjaman.dipinjam ? "Belum Dikembalikan" : "Sudah Dikembalikan", 18) << " |\n";
                cout << "|--------------------------------------------------------------|\n";
                current = current->next;
            }
        }
    }

    static string padRight(const string& str, size_t width) {
        if (str.length() >= width) {
            return str;
        } else {
            return str + string(width - str.length(), ' ');
        }
    }
};

class PohonBuku
{
private:
    Buku *akar;
    HashTable hashTable;

    Buku *tambahBuku(Buku *node, string judul, string pengarang, int tahunTerbit, int stok, string rak, int nomorKotak)
    {
        if (node == nullptr)
        {
            return new Buku(judul, pengarang, tahunTerbit, stok, rak, nomorKotak);
        }
        if (judul < node->judul)
        {
            node->kiri = tambahBuku(node->kiri, judul, pengarang, tahunTerbit, stok, rak, nomorKotak);
        }
        else if (judul > node->judul)
        {
            node->kanan = tambahBuku(node->kanan, judul, pengarang, tahunTerbit, stok, rak, nomorKotak);
        }
        else
        {
            node->stok += stok;
        }
        return node;
    }

    pair<Buku *, string> cariBukuDenganLokasi(Buku *node, string judul)
    {
        if (node == nullptr || node->judul == judul)
        {
            return {node, ""};
        }
        if (judul < node->judul)
        {
            return cariBukuDenganLokasi(node->kiri, judul);
        }
        return cariBukuDenganLokasi(node->kanan, judul);
    }

    void gatherInOrder(Buku *node, Buku *semuaBuku[], int &index)
    {
        if (node != nullptr)
        {
            gatherInOrder(node->kiri, semuaBuku, index);
            semuaBuku[index++] = node;
            gatherInOrder(node->kanan, semuaBuku, index);
        }
    }

    void merge(Buku *arr[], int low, int mid, int high)
    {
        int n1 = mid - low + 1;
        int n2 = high - mid;

        Buku *L[n1];
        Buku *R[n2];

        for (int i = 0; i < n1; ++i)
            L[i] = arr[low + i];
        for (int j = 0; j < n2; ++j)
            R[j] = arr[mid + 1 + j];

        int i = 0, j = 0, k = low;
        while (i < n1 && j < n2)
        {
            if (L[i]->tahunTerbit <= R[j]->tahunTerbit)
            {
                arr[k] = L[i];
                ++i;
            }
            else
            {
                arr[k] = R[j];
                ++j;
            }
            ++k;
        }
        while (i < n1)
        {
            arr[k] = L[i];
            ++i;
            ++k;
        }
        while (j < n2)
        {
            arr[k] = R[j];
            ++j;
            ++k;
        }
    }

    void mergeSort(Buku *arr[], int low, int high)
    {
        if (low < high)
        {
            int mid = low + (high - low) / 2;
            mergeSort(arr, low, mid);
            mergeSort(arr, mid + 1, high);
            merge(arr, low, mid, high);
        }
    }

public:
    PohonBuku()
    {
        akar = nullptr;
    }
    void tambahBuku(string judul, string pengarang, int tahunTerbit, int stok, string rak, int nomorKotak)
    {
        akar = tambahBuku(akar, judul, pengarang, tahunTerbit, stok, rak, nomorKotak);
    }
    pair<Buku *, string> cariBukuDenganLokasi(string judul)
    {
        return cariBukuDenganLokasi(akar, judul);
    }
    void tampilBukuMenurutTahun()
    {
        cout << "=======================================\n";
        cout << "Buku diurutkan berdasarkan tahun terbit\n";
        cout << "=======================================\n";

        Buku *semuaBuku[100];
        int count = 0;
        gatherInOrder(akar, semuaBuku, count);

        if (count == 0)
        {
            cout << "Tidak ada data buku yang tersedia.\n";
            return;
        }
        mergeSort(semuaBuku, 0, count - 1);
        for (int i = 0; i < count; ++i)
        {
            cout << "|Judul       : " << padRight(semuaBuku[i]->judul, 23) << " |\n";
            cout << "|Pengarang   : " << padRight(semuaBuku[i]->pengarang, 23) << " |\n";
            cout << "|Tahun Terbit: " << padRight(to_string(semuaBuku[i]->tahunTerbit), 23) << " |\n";
            cout << "|Stok        : " << padRight(to_string(semuaBuku[i]->stok), 23) << " |\n";
            cout << "|--------------------------------------|\n";
        }
    }
    string padRight(const string &str, size_t width)
    {
        if (str.length() >= width)
        {
            return str;
        }
        else
        {
            return str + string(width - str.length(), ' ');
        }
    }

    void pinjamBuku(string namaPeminjam, string judulBuku) {
        pair<Buku*, string> result = cariBukuDenganLokasi(akar, judulBuku);
        Buku* buku = result.first;
        if (buku == nullptr) {
            cout << "Buku tidak ditemukan." << endl;
            return;
        }
        if (buku->stok <= 0) {
            cout << "Maaf, Stok buku \"" << judulBuku << "\" habis." << endl;
            return;
        }
        buku->stok--;
        hashTable.addPeminjaman(Peminjaman(namaPeminjam, judulBuku));
        cout << "Buku \"" << judulBuku << "\" berhasil dipinjam oleh " << namaPeminjam << ".\n";
    }
    void kembalikanBuku(string namaPeminjam, string judulBuku) {
        pair<Buku*, string> result = cariBukuDenganLokasi(akar, judulBuku);
        Buku* buku = result.first;
        if (buku == nullptr) {
             cout << "Maaf, buku \"" << judulBuku << "\" tidak terdaftar dalam perpustakaan.\n";
            return;
        }
        buku->stok++;
        if (hashTable.ubahStatusPeminjaman(namaPeminjam, judulBuku)) {
            cout << "Buku \"" << judulBuku << "\" telah dikembalikan oleh " << namaPeminjam << ".\n";
        } else {
            cout << "Maaf, buku \"" << judulBuku << "\" tidak dipinjam oleh " << namaPeminjam << ".\n";
        }
    }

    void detailPeminjaman() {
    hashTable.printPeminjaman();
}

};
class Graph
{
private:
    static const int MAX_VERTICES = 100;
    string vertices[MAX_VERTICES];
    bool adjacencyMatrix[MAX_VERTICES][MAX_VERTICES];

    int vertexCount;
    int findIndex(const string &vertex)
    {
        for (int i = 0; i < vertexCount; ++i)
        {
            if (vertices[i] == vertex)
            {
                return i;
            }
        }
        return -1;
    }

public:
    Graph()
    {
        vertexCount = 0;
        for (int i = 0; i < MAX_VERTICES; ++i)
        {
            for (int j = 0; j < MAX_VERTICES; ++j)
            {
                adjacencyMatrix[i][j] = false;
            }
        }
    }
    void addVertex(const string &vertex)
    {
        vertices[vertexCount++] = vertex;
    }
    void addEdge(const string &source, const string &destination)
    {
        int sourceIndex = findIndex(source);
        int destIndex = findIndex(destination);
        if (sourceIndex != -1 && destIndex != -1)
        {
            adjacencyMatrix[sourceIndex][destIndex] = true;
            adjacencyMatrix[destIndex][sourceIndex] = true;
        }
    }
};

class ManajemenAnggota
{
private:
    static const int MAX_ANGGOTA = 100;
    Anggota *daftarAnggota[MAX_ANGGOTA];
    int jumlahAnggota = 0;
    PohonBuku *pohonBuku;

public:

    ManajemenAnggota(PohonBuku *pohon) : pohonBuku(pohon)
    {
        for (int i = 0; i < MAX_ANGGOTA; ++i)
        {
            daftarAnggota[i] = nullptr;
        }
    }

    Anggota *verifikasiLogin(const string &nama, const string &password)
    {
        for (int i = 0; i < jumlahAnggota; ++i)
        {
            if (daftarAnggota[i]->nama == nama && daftarAnggota[i]->password == password)
            {
                return daftarAnggota[i];
            }
        }
        return nullptr;
    }

    void buatAkun(const string &nama, const string &password, const string &email, const string &nomorKontak)
    {
        if (jumlahAnggota < MAX_ANGGOTA)
        {
            daftarAnggota[jumlahAnggota] = new Anggota{nama, email, nomorKontak, password};
            ++jumlahAnggota;
            cout << "Akun anggota berhasil dibuat.\n";
        }
        else
        {
            cout << "Batas jumlah anggota telah tercapai. Tidak dapat membuat akun baru.\n";
        }
    }

    void pinjamBuku(Anggota *anggota, string judul)
    {
        if (anggota != nullptr)
        {
            pohonBuku->pinjamBuku(anggota->nama, judul);
        }
        else
        {
            cout << "Anggota dengan nama tersebut tidak ditemukan.\n";
        }
    }

    void kembalikanBuku(Anggota *anggota, string judul)
    {
        if (anggota != nullptr)
        {
            pohonBuku->kembalikanBuku(anggota->nama, judul);
        }
        else
        {
            cout << "Anggota dengan nama tersebut tidak ditemukan.\n";
        }
    }
};
int main()
{
    PohonBuku perpustakaan;
    Graph libraryGraph;
    ManajemenAnggota manajemenAnggota(&perpustakaan);
    int pilihan;
    do
    {
        cout << "\nSistem Manajemen Perpustakaan\n";
        cout << "1. Tambah Buku\n";
        cout << "2. Cari Buku\n";
        cout << "3. Tampilkan Buku berdasarkan Tahun Terbit\n";
        cout << "4. Detail Peminjaman Buku\n";
        cout << "5. Login Anggota\n";
        cout << "6. Keluar\n";
        cout << "Masukkan pilihan Anda: ";
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
        {
            string judul, pengarang, rak;
            int tahunTerbit, stok, nomorKotak;
            cout << "Masukkan judul buku: ";
            cin.ignore();
            getline(cin, judul);
            cout << "Masukkan pengarang: ";
            getline(cin, pengarang);
            cout << "Masukkan tahun terbit: ";
            cin >> tahunTerbit;
            cout << "Masukkan stok: ";
            cin >> stok;
            cout << "Masukkan rak tempat buku: ";
            cin.ignore();
            getline(cin, rak);
            cout << "Masukkan nomor kotak tempat buku: ";
            cin >> nomorKotak;
            perpustakaan.tambahBuku(judul, pengarang, tahunTerbit, stok, rak, nomorKotak);
            break;
        }
        case 2:
        {
            string judul;
            cout << "Masukkan judul untuk dicari: ";
            cin.ignore();
            getline(cin, judul);
            pair<Buku *, string> hasilPencarian = perpustakaan.cariBukuDenganLokasi(judul);
            if (hasilPencarian.first != nullptr)
            {
                cout << "==============\n";
                cout << "Buku ditemukan\n";
                cout << "==============\n";
                cout << "Judul        : " << hasilPencarian.first->judul << "\n";
                cout << "Pengarang    : " << hasilPencarian.first->pengarang << "\n";
                cout << "Tahun Terbit : " << hasilPencarian.first->tahunTerbit << "\n";
                cout << "Stok         : " << hasilPencarian.first->stok << "\n";
                cout << "Lokasi Buku  : " << "Rak " << hasilPencarian.first->rak << ", Kotak " << hasilPencarian.first->nomorKotak << endl;
            }
            else
            {
                cout << "Buku tidak ditemukan.\n";
            }
            break;
        }
        case 3:
            perpustakaan.tampilBukuMenurutTahun();
            break;
        case 4:
            perpustakaan.detailPeminjaman();
            break;
        case 5:
        {
            int opsiMenuAnggota;
            cout << "Menu Anggota\n";
            cout << "1. Login\n";
            cout << "2. Buat Akun\n";
            cout << "Masukkan pilihan Anda: ";
            cin >> opsiMenuAnggota;

            switch (opsiMenuAnggota)
            {
            case 1:
            {
                string nama, password;
                cout << "Masukkan nama: ";
                cin >> nama;
                cout << "Masukkan password: ";
                cin >> password;
                Anggota *anggota = manajemenAnggota.verifikasiLogin(nama, password);
                if (anggota != nullptr)
                {
                    int pilihanAnggota;
                    do
                    {
                        cout << "\nMenu Anggota\n";
                        cout << "1. Pinjam Buku\n";
                        cout << "2. Kembalikan Buku\n";
                        cout << "3. Keluar\n";
                        cout << "Masukkan pilihan Anda: ";
                        cin >> pilihanAnggota;

                        switch (pilihanAnggota)
                        {
                        case 1:
                        {
                            string judul;
                            cout << "Masukkan judul buku yang ingin dipinjam: ";
                            cin.ignore();
                            getline(cin, judul);
                            manajemenAnggota.pinjamBuku(anggota, judul);
                            break;
                        }
                        case 2:
                        {
                            string judul;
                            cout << "Masukkan judul buku yang ingin dikembalikan: ";
                            cin.ignore();
                            getline(cin, judul);
                            manajemenAnggota.kembalikanBuku(anggota, judul);
                            break;
                        }
                        case 3:
                            cout << "Keluar...\n";
                            break;
                        default:
                            cout << "Pilihan tidak valid.\n";
                        }
                    } while (pilihanAnggota != 3);
                }
                else
                {
                    cout << "Nama pengguna atau password salah.\n";
                }
                break;
            }
            case 2:
            {
                string nama, password, email, nomorKontak;
                cout << "Masukkan nama: ";
                cin >> nama;
                cout << "Masukkan email: ";
                cin >> email;
                cout << "Masukkan nomor kontak: ";
                cin >> nomorKontak;
                cout << "Masukkan password: ";
                cin >> password;
                manajemenAnggota.buatAkun(nama, password, email, nomorKontak);
                cout << "Akun berhasil dibuat. Silakan login kembali.\n";
                continue;
            }
            default:
                cout << "Pilihan tidak valid.\n";
            }
            break;
        }
        case 6:
            cout << "Keluar...\n";
            break;
        default:
            cout << "Pilihan tidak valid. Silakan masukkan angka antara 1 dan 7.\n";
        }
    } while (pilihan != 7);
    return 0;
}
