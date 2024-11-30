#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Kullanıcı bilgileri için struct tanımı
typedef struct {
    char hesapNo[8];       // Hesap numarası
    char isim[50];         // Kullanıcı adı
    char sifre[5];         // Şifre (4 basamaklı)
    float bakiye;          // Kullanıcının bakiyesi
    char iban[27];         // IBAN numarası (26 haneli + 1 byte ekstra)
    int blokDurumu;        // Hesap bloke durumu (0: aktif, 1: bloke)
} Kullanici;

// Kullanıcı dizisini global olarak tanımla
Kullanici kullanicilar[5] = {
    {"1234567", "Ahmet", "1234", 5000.0, "TR123456789012345678901234", 0},
    {"2345678", "Mehmet", "5678", 3000.0, "TR234567890123456789012345", 0},
    {"3456789", "Elif", "9101", 10000.0, "TR345678901234567890123456", 0},
    {"4567890", "Ayse", "4321", 7500.0, "TR456789012345678901234567", 0},
    {"5678901", "Fatma", "8765", 2500.0, "TR567890123456789012345678", 0}
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
        printf("Hesap Numaranizi Giriniz (7 haneli): ");
        scanf("%s", hesapNo);

        // Hesap numarasının uzunluğunu kontrol et
        if (strlen(hesapNo) != 7) {
            printf("Hesap numarasi 7 hanelidir.\n");
            continue; // Hesap numarası hatalıysa döngüye devam et
        }

        printf("Adinizi Giriniz: ");
        scanf("%s", isim);

        for (i = 0; i < 5; i++) {
            if (strcmp(kullanicilar[i].hesapNo, hesapNo) == 0 &&
                strcmp(kullanicilar[i].isim, isim) == 0) {

                if (kullanicilar[i].blokDurumu == 1) {
                    printf("Hesabiniz bloke olmus. Musteri hizmetleri ile iletisime gecin.\n");
                    return 0;
                }

                hak = 3; // Şifre giriş hakkı
                while (hak > 0) {
                    printf("Sifrenizi Giriniz (4 basamakli): ");
                    scanf("%s", sifre);

                    if (strcmp(kullanicilar[i].sifre, sifre) == 0) {
                        printf("Giris Basarili!\n");
                        logKaydet(hesapNo, "Basarili Giris");
                        girisBasarili = 1;
                        menu(&kullanicilar[i]);
                        break;
                    } else {
                        hak--;
                        if (hak == 0) {
                            kullanicilar[i].blokDurumu = 1;
                            printf("Hesabiniz bloke edildi. Musteri hizmetleri ile iletisime gecin.\n");
                            logKaydet(hesapNo, "Bloke Edildi");
                            return 0;
                        }
                        // Hata mesajını ve kalan hakkı alt alta yazdırma
                        printf("Yanlis sifre girdiniz.\n");
                        printf("Tekrar deneyiniz. Kalan hak: %d\n", hak);
                    }
                }

                break;
            }
        }

        if (girisBasarili) {
            break; // Giriş başarılıysa döngüyü sonlandır
        } else {
            printf("Hesap bulunamadi veya bilgiler hatali. Tekrar deneyin.\n");
        }
    }

    return 0;
}

// Ana Menü fonksiyonu
void menu(Kullanici *kullanici) {
    int secim;
    do {
        printf("\n--- ATM Menu ---\n");
        printf("1. Para Cekme\n");
        printf("2. Para Yatirma\n");
        printf("3. Bakiye Goruntuleme\n");
        printf("4. Para Transferi\n");
        printf("5. Cikis\n");
        printf("Seciminizi yapiniz: ");
        scanf("%d", &secim);

        switch (secim) {
            case 1: paraCekme(kullanici); break;
            case 2: paraYatirma(kullanici); break;
            case 3: bakiyeGoruntuleme(kullanici); break;
            case 4: paraTransferi(kullanici); break;
            case 5: printf("Cikis yapiliyor...\n"); logKaydet(kullanici->hesapNo, "Cikis"); break;
            default: printf("Gecersiz secim, tekrar deneyin.\n");
        }
    } while (secim != 5);
}

// Para çekme fonksiyonu
void paraCekme(Kullanici *kullanici) {
    float miktar;
    printf("Cekmek istediginiz miktari giriniz: ");
    scanf("%f", &miktar);

    if (miktar > kullanici->bakiye) {
        printf("Yetersiz bakiye.\n");
        logKaydet(kullanici->hesapNo, "Basarisiz Para Cekme");
    } else {
        kullanici->bakiye -= miktar;
        printf("Para cekildi. Guncel bakiye: %.2f\n", kullanici->bakiye);
        logKaydet(kullanici->hesapNo, "Basarili Para Cekme");
    }
}

// Para yatırma fonksiyonu
void paraYatirma(Kullanici *kullanici) {
    float miktar;
    printf("Yatirmak istediginiz miktari giriniz: ");
    scanf("%f", &miktar);

    kullanici->bakiye += miktar;
    printf("Para yatirildi. Guncel bakiye: %.2f\n", kullanici->bakiye);
    logKaydet(kullanici->hesapNo, "Para Yatirma");
}

// Bakiye görüntüleme fonksiyonu
void bakiyeGoruntuleme(Kullanici *kullanici) {
    printf("Mevcut bakiyeniz: %.2f\n", kullanici->bakiye);
    logKaydet(kullanici->hesapNo, "Bakiye Goruntuleme");
}

// Para transferi fonksiyonu
void paraTransferi(Kullanici *kullanici) {
    char aliciIBAN[27], aliciIsim[50];
    float transferMiktari;
    int i, bulunan = 0;

    while (1) {
        printf("Para transferi yapmak icin alicinin IBAN numarasini giriniz (TR ile baslayan 26 haneli): ");
        scanf("%s", aliciIBAN);

        // IBAN numarasının doğruluğunu kontrol et (TR ile başlayıp 26 haneli olmalı)
        if (strncmp(aliciIBAN, "TR", 2) != 0 || strlen(aliciIBAN) != 26) {
            printf("Gecersiz IBAN numarasi. Lutfen TR ile baslayan 26 haneli IBAN numarasi giriniz.\n");
            continue;
        }

        // Kendi IBAN'ına para gönderme kontrolü
        if (strcmp(kullanici->iban, aliciIBAN) == 0) {
            printf("Kendi IBAN adresinize para gonderemezsiniz. Lutfen baska bir IBAN giriniz.\n");
            continue;  // Geçersiz IBAN durumunda tekrar IBAN girilmesi istenir
        }

        // IBAN geçerli ve kullanıcı kendi IBAN'ı değilse döngüden çık
        break;
    }

    // Alıcı ismini sormadan önce tamponu temizle
    getchar(); // Bu, önceden girilen yeni satır karakterini temizler

    // Alıcı ismini sorma
    printf("Alici: ");
    scanf("%s", aliciIsim);

    // Alıcıyı bul
    for (i = 0; i < 5; i++) {
        if (strcmp(kullanicilar[i].iban, aliciIBAN) == 0 && strcmp(kullanicilar[i].isim, aliciIsim) == 0) {
            bulunan = 1;  // Alıcı bulundu
            break;
        }
    }

    if (!bulunan) {
        printf("Alıcı bulunamadı veya yanlış bilgiler girildi.\n");
        return;
    }

    // Transfer miktarı soruluyor
    printf("Transfer miktarini giriniz: ");
    scanf("%f", &transferMiktari);

    if (transferMiktari > kullanici->bakiye) {
        printf("Yetersiz bakiye.\n");
    } else {
        kullanici->bakiye -= transferMiktari; // Gönderen kullanıcının bakiyesinden düşülüyor
        kullanicilar[i].bakiye += transferMiktari; // Alıcıya bakiyesi ekleniyor
        printf("Transfer basarili!\n Yeni bakiye: %.2f\n", kullanici->bakiye);
        logKaydet(kullanici->hesapNo, "Basarili Para Transferi");
    }
}

// Log kaydetme fonksiyonu
void logKaydet(const char *hesapNo, const char *islem) {
    FILE *logFile = fopen("atm_log.txt", "a");
    time_t t;
    time(&t);
    fprintf(logFile, "%s - Hesap No: %s, Islem: %s\n", ctime(&t), hesapNo, islem);
    fclose(logFile);
}
