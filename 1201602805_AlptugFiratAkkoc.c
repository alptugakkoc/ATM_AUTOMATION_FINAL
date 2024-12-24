// 1201602805 Alptuğ Fırat Akkoç

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct { // Kullanıcı bilgileri için struct tanım
    char hesapNo[8];       // Kullanıcı Hesap numarası
    char isim[50];         // Kullanıcı adı
    char sifre[5];         // Şifre (4 basamaklı)
    float bakiye;          // Kullanıcının bakiyesi
    char iban[27];         // IBAN numarası (26 haneli)
    int blokDurumu;        // Hesap bloke durumu 
} Kullanici;

// Kullanıcı dizisini global olarak tanımladım,veritabanı bağlantısı eklemediğim için atm giriş yapacak kullanıcıları ben belirledim
Kullanici kullanicilar[5] = {
    {"1111111", "Alptug", "1212", 6000.25, "TR111111111111111111111111", 0},
    {"2222222", "Firat", "2323", 7000.45, "TR222222222222222222222222", 0},
    {"3333333", "Semra", "3434", 20000.55, "TR333333333333333333333333", 0},
    {"4444444", "Fikret", "4545", 3500.15, "TR444444444444444444444444", 0},
    {"5555555", "Cansu", "5656", 6500.35, "TR555555555555555555555555", 0}
};

// Fonksiyon prototipleri
void menu(Kullanici *kullanici);
void paraCekme(Kullanici *kullanici);
void paraYatirma(Kullanici *kullanici);
void bakiyeGoruntuleme(Kullanici *kullanici);
void paraTransferi(Kullanici *kullanici);
void logKaydet(const char *hesapNo, const char *islem);

int main() {
    char hesapNo[8], isim[50], sifre[5];
    int girisBasarili = 0, i, hak;

    while (1) {
        printf("Hesap Numaranizi Giriniz (7 haneli): "); // Global tanımladığımız kullanıcı hesap numarası ile giriş
        scanf("%s", hesapNo);

        // Hesap numarasının uzunluğunu kontrol etme
        if (strlen(hesapNo) != 7) {
            printf("Hesap numarasi 7 hanelidir.\n");
            continue;
        }

        printf("Adinizi Giriniz: ");   // Hesap numarası ile eşleşen isim girişi
        scanf("%s", isim);

        for (i = 0; i < 5; i++) { // Hesap No ile isim eşleşen kullanıcıyı buluyorum
            if (strcmp(kullanicilar[i].hesapNo, hesapNo) == 0 &&
                strcmp(kullanicilar[i].isim, isim) == 0) {

                if (kullanicilar[i].blokDurumu == 1) { // Kullanıcının hesabının bloke olup olmadığını kontrol ediyorum eüer bloklu durum varsa uyarı mesajı görüntüleniyor
                    printf("Hesabiniz bloke olmus. Musteri hizmetleri ile iletisime gecin.\n");
                    return 0;
                }

                hak = 3; // Kullanıcının ATM giriş yapabilmesi için 3 şifre deneme hakkı var 3 ve üstü olduğunda hesabı bloke durumuna düşecek ve hata mesajıyla karşılaşacak
                while (hak > 0) {
                    printf("Sifrenizi Giriniz (4 basamakli): "); // global olarak tanımladığım şifre ile giriş yapılmalı
                    scanf("%s", sifre);

                    if (strcmp(kullanicilar[i].sifre, sifre) == 0) { // Şifre eşleşirse başarılı giriş gerçekleştirilip , atm girişi gerçekleşiyor
                        printf("Giris Basarili!\n");
                        logKaydet(hesapNo, "Basarili Giris");
                        girisBasarili = 1;
                        menu(&kullanicilar[i]);
                        break;
                    } else {
                        hak--; // Yanlış şifre durumunda hakkımız 1 düştü
                        if (hak == 0) {
                            kullanicilar[i].blokDurumu = 1;
                            printf("Hesabiniz bloke edildi. Musteri hizmetleri ile iletisime gecin.\n"); // 3 defa yanlış şifre girişi yapan kullanıcının karşılaştığı hata mesajı
                            logKaydet(hesapNo, "Bloke Edildi"); // Kullanıcının yapmış olduğu hareketleri Log dosyası olarak kayıt ediyoruz , devam eden kodlarda açıklıyorum.
                            return 0;
                        }

                        printf("Yanlis sifre girdiniz.\n");  // Yanlış şifre giren kullanıcıya uyarı mesajı
                        printf("Tekrar deneyiniz. Kalan hak: %d\n", hak); // Kalan hak 3den geriye doğru düşerek yeniden giriş imkanı sağladık.
                    }
                }

                break;
            }
        }

        if (girisBasarili) {
            break; // Giriş başarılıysa döngü sonlandırıldı
        } else {
            printf("Hesap bulunamadi veya bilgiler hatali. Tekrar deneyin.\n"); // Kullanıcı hesap no ile isim eşleşmediği zaman karşılaşılan uyarı mesajı
        }
    }

    return 0;
}


void menu(Kullanici *kullanici) { // Ana Menü fonksiyonu
    int secim;
    do {
        printf("\n--- ATM Menu ---\n"); // Menü içerikleri
        printf("1. Para Cekme\n");      // Menü içerikleri
        printf("2. Para Yatirma\n");    // Menü içerikleri
        printf("3. Para Transferi\n");  // Menü içerikleri
        printf("4. Bakiye Goruntuleme\n");  // Menü içerikleri
        printf("5. Cikis\n");               // Menü içerikleri
        printf("Seciminizi yapiniz: ");     // Menü içerikleri
        scanf("%d", &secim);

        switch (secim) {  // seçim işlemlerini switch case ile gerçekleştirdim
            case 1: paraCekme(kullanici); break;
            case 2: paraYatirma(kullanici); break;
            case 3: paraTransferi(kullanici); break;
            case 4: bakiyeGoruntuleme(kullanici); break;
            case 5: printf("Cikis yapiliyor...\n"); logKaydet(kullanici->hesapNo, "Cikis"); break; // Kullanıcının hareketlerini atm_log.txt dosyasına yazdırıyorum
            default: printf("Gecersiz secim, tekrar deneyin.\n");
        }
    } while (secim != 5); // Kullanıcı 5 ile çıkış yapana kadar do while döngüm devam edicek
}


void paraCekme(Kullanici *kullanici) { // Para çekme fonksiyonu
    float miktar;    // Para tutarı 500,45 kuruş gibi rakam olabileceği için float tanımladım
    printf("Cekmek istediginiz miktari giriniz: "); // Kullanıcıdan çekim yapmak istediği miktarı alıyorum
    scanf("%f", &miktar);

    if (miktar > kullanici->bakiye) { // Eğer çekim yapılcak bakiye kullanıcı hesabındakinden fazla ise yetersiz bakiye mesajı görüntülüyorum.(Bakiyeyi global olarak ben tanımladım)
        printf("Yetersiz bakiye.\n");
        logKaydet(kullanici->hesapNo, "Basarisiz Para Cekme"); // Kullanıcının hareketini atm_log.txt dosyasına kayıt ettim
    } else {
        kullanici->bakiye -= miktar; // Çekim yapılcak bakiye hesapta bulunandan az ise başarılı işlem gerçekleşti, Para çekildi , Güncel bakiye mesajı görüntülendi.
        printf("Para cekildi. Guncel bakiye: %.2f\n", kullanici->bakiye);
        logKaydet(kullanici->hesapNo, "Basarili Para Cekme"); // Kullanıcının hareketini atm_log.txt dosyasına kayıt ettim
    }
}


void paraYatirma(Kullanici *kullanici) {// Para yatırma fonksiyonu
    float miktar;
    printf("Yatirmak istediginiz miktari giriniz: "); // Kullanıcıdan yatırmak istediği miktar bilgisini aldım
    scanf("%f", &miktar);

    kullanici->bakiye += miktar; // Kullanıcının yatırdığı bakiye mevcud bakıyeye eklenerek yeni bakiye oluşturuldu
    printf("Para yatirildi. Guncel bakiye: %.2f\n", kullanici->bakiye); // Para yatırdı mesajı ve güncel bakiye görüntülenmesi sağlandı
    logKaydet(kullanici->hesapNo, "Para Yatirma"); // Kullanıcının hareketini atm_log.txt dosyasına kayıt ettim
}

void paraTransferi(Kullanici *kullanici) {// Para transferi fonksiyonu eklemek istedim
    char aliciIBAN[27], aliciIsim[50];  // Para transferi için iban numarasını char olarak tanımladım (TR strandartlarına göre 26 basamaklı)
    float transferMiktari;
    int i, bulunan = 0;

    while (1) { // Kullanıcı doğru iban girene kadar döngü devam edicek
        printf("Para transferi yapmak icin alicinin IBAN numarasini giriniz (TR ile baslayan 26 haneli): "); // Kullanıcıdan transver ibanını aldım (ibanları global olarak ben tanımladım)
        scanf("%s", aliciIBAN);


        if (strncmp(aliciIBAN, "TR", 2) != 0 || strlen(aliciIBAN) != 26) { // IBAN numarasının doğruluğunu kontrolü (TR ile başlayıp 26 haneli olmalı)
            printf("Gecersiz IBAN numarasi. Lutfen TR ile baslayan 26 haneli IBAN numarasi giriniz.\n");
            continue; // Döngüyü başa alıp tekrar iban istiyorum
        }


        if (strcmp(kullanici->iban, aliciIBAN) == 0) { // Eğer kullanıcı kendi ibanına para göndermeye çalışırsa hata mesajı alacak
            printf("Kendi IBAN adresinize para gonderemezsiniz. Lutfen baska bir IBAN giriniz.\n");
            continue;  // Döngü başına dönerek yeni iban girmesini ister
        }

        break;
    }

    getchar();


    printf("Alici: "); // İban alıcı ismini kullanısıya soruyorum
    scanf("%s", aliciIsim);


    for (i = 0; i < 5; i++) { // Alıcıyı bulma fonksiyonu ( alıcı tanımladıgım kullanıcılardan birisi)
        if (strcmp(kullanicilar[i].iban, aliciIBAN) == 0 && strcmp(kullanicilar[i].isim, aliciIsim) == 0) { // Kullanıcının ibanı doğru bir şekilde eşleşti mi sorgusu
            bulunan = 1;  // Alıcı doğru bir şekilde bulundu
            break;
        }
    }

    if (!bulunan) { // İban ile isim eşleşmediği zaman hata alıyoruz
        printf("Alıcı bulunamadı veya yanlış bilgiler girildi.\n");
        return;
    }


    printf("Transfer miktarini giriniz: ");// Transfer miktarını kullanıcıdan alıyorum
    scanf("%f", &transferMiktari);

    if (transferMiktari > kullanici->bakiye) { // Transfer miktarı , bakiyeden az ise hata mesajı görüntüleriz
        printf("Yetersiz bakiye.\n");
    } else {
        kullanici->bakiye -= transferMiktari; // Transfer gerçekleşti ve miktar bakiyeden düşülür
        kullanicilar[i].bakiye += transferMiktari; // Alıcıya bakiyesine transfer eklenir
        printf("Transfer basarili!\n Yeni bakiye: %.2f\n", kullanici->bakiye); // Başarılı transfer mesajı ve mevcud bakiye görüntülenir
        logKaydet(kullanici->hesapNo, "Basarili Para Transferi");  // Kullanıcının hareketini atm_log.txt dosyasına kayıt ettim
    }
}

void bakiyeGoruntuleme(Kullanici *kullanici) {//Kullanıcının bakiyesini görüntüleme fonksiyonu
    printf("Mevcut bakiyeniz: %.2f\n", kullanici->bakiye);
    logKaydet(kullanici->hesapNo, "Bakiye Goruntuleme"); // Kullanıcının hareketini atm_log.txt dosyasına kayıt ettim
}



void logKaydet(const char *hesapNo, const char *islem) {//Kullanıcı işlemlerini kaydetme fonksiyonu
    FILE *logFile = fopen("atm_log.txt", "a");  // atm_log.txt adında bir dosya oluşturdum ve mod olarak append (a) kullandım
    time_t t; // Zaman bilgisini saklamak için kullandım
    time(&t); // Mevcud zaman bilgisi için
    fprintf(logFile, "%s - Hesap No: %s, Islem: %s\n", ctime(&t), hesapNo, islem); //Hesap no ve işlemin ne olduğunu tarihle birlikte dosyaya yazdırdım
    fclose(logFile); // Dosya kapatma işlemi
}
// Kodun çıktısı aşamalı olarak
/*
Hesap Numaranizi Giriniz (7 haneli):1111111
 Adinizi Giriniz:Alptug
 Sifrenizi Giriniz (4 basamakli):1212
 Giris Basarili!

--- ATM Menu ---
1. Para Cekme
2. Para Yatirma
3. Para Transferi
4. Bakiye Goruntuleme
5. Cikis
Seciminizi yapiniz:1
 Cekmek istediginiz miktari giriniz:400
 Para cekildi. Guncel bakiye: 5600.25

--- ATM Menu ---
1. Para Cekme
2. Para Yatirma
3. Para Transferi
4. Bakiye Goruntuleme
5. Cikis
Seciminizi yapiniz:2
 Yatirmak istediginiz miktari giriniz:500
 Para yatirildi. Guncel bakiye: 6100.25

--- ATM Menu ---
1. Para Cekme
2. Para Yatirma
3. Para Transferi
4. Bakiye Goruntuleme
5. Cikis
Seciminizi yapiniz:4
 Mevcut bakiyeniz: 6100.25

--- ATM Menu ---
1. Para Cekme
2. Para Yatirma
3. Para Transferi
4. Bakiye Goruntuleme
5. Cikis
Seciminizi yapiniz:5
 Cikis yapiliyor...
 */