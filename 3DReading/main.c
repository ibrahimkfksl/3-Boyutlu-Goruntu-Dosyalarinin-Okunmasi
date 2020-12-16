#include <stdio.h>
#include <stdlib.h>
#include <Math.h>
#include <string.h>
#include <dirent.h>
// IBRAHIM KAFKASLI 180201078 -- ABDULLAH YASAR KISA 180201015
struct noktalar
{
    float x;
    float y;
    float z;
    int r;
    int g;
    int b;
};
struct noktalar2
{
    float x;
    float y;
    float z;
};
struct sonsecim
{
    double nokta_uzaklik;
    double toplam_kiyaslama;
    double denklem;
};
int main()
{
    DIR *dizin;
    struct dirent *girdi;
    //char menu[166]= {}; //hata alindigi icin boyle yapildi
    char satir1[30],satir2[30],satir3[30],satir4[30],bos_satir[50],satir5[30];
    int sayi,version;
    int nokta_sayisi=0;
    float x,y,z,x2,y2,z2;
    int r,g,b,r2,g2,b2;
    char tmp_sayi[50];
    int hata=0;

    for(int i=0; i<5; i++)
    {
        printf("\n1-Dosya Kontrolu Yapma\n2-En Yakin/En Uzak Noktalarin Konumlari\n3-Kupun Nokta Bilgileri\n4-Kurenin Icine Aldigi Noktalar\n5-Nokta Uzaklik Ortalamalari\nBir Secim yapiniz: ");
        int secim;
        scanf("%d",&secim);
        if(i==0 && secim!=1) //1. secenege ilk tiklanmadigi surece dongu surekli doner
        {
            printf("\nLutfen ilk 1'i tuslayiniz!\n");
            i=i-1;
            continue;
        }
        if(secim==1)
        {

            dizin=opendir(".");//dizine giriliyor
            if(dizin==NULL)
            {
                printf("Dizin Okunamadi!\n");
                return(1);
            }
            while((girdi=readdir(dizin)))//dizinin icinde dolaniyor
            {
                char dosya_adi[]= {};//cekilen .nkt uzantili dosyanin adini tutucak
                strcpy(dosya_adi,(girdi->d_name));//dizindeki dosya isimlerini tutuyor

                if(strstr(dosya_adi,"output")==NULL && strstr(dosya_adi,".nkt")!=NULL)//dosya adinda output gecmemeli ve .nkt uzantili olmali
                {
                    int satir=0;
                    nokta_sayisi=0;//nokta sayisini tutuyor
                    FILE *dptrin;
                    FILE *dptrout;//dosyalar tanimlaniyor
                    char output_adi[35]="output_";//output adini tutuyor
                    strcat(output_adi,dosya_adi);
                    dptrout=fopen(output_adi,"w");//yazma modu (output'a)
                    dptrin=fopen(dosya_adi,"r+b");//okuma modu binary dahil
                    fprintf(dptrout,"SECIM 1\n");
                    hata=0; //tum dosyalar hatasiz kabul ediliyor
                    if(dptrin==NULL)
                    {
                        printf("%s acilamadi\n",dosya_adi);
                    }
                    else
                    {
                        rewind(dptrin);//imleci basa al
                        fgets(satir1,30,dptrin);
                        fscanf(dptrin,"%s%d",satir2,&version);
                        fgets(bos_satir,29,dptrin);//fscanf in hatasindan dolayi bosluk karakteri icin yazildi
                        fgets(satir3,29,dptrin);
                        fscanf(dptrin,"%s%d",satir4,&sayi);
                        fgets(bos_satir,29,dptrin);
                        fgets(satir5,29,dptrin);
                        rewind(dptrin); //dosyanin ilk bes satirini cekme kodlari...
                        while(!feof(dptrin))
                        {
                            if(fgetc(dptrin)=='\n')
                            {
                                nokta_sayisi++;
                            }
                        }
                        nokta_sayisi=nokta_sayisi-5;//\n sayisindan ilk 5 satirin \n leri cikartiliyor
                        if(nokta_sayisi!=sayi && satir5=="DATA ascii\n")//ascii tipli dosyalar icin nokta sayisi kontrolu
                        {
                            hata=-1;
                            fprintf(dptrout,"%s Dosyasindaki Nokta Bilgileri Hatalidir\n",dosya_adi);
                        }
                        if(version!=1)//tum dosyalar icin versiyon sayisi kontrolu
                        {
                            fprintf(dptrout,"%s Dosyasindaki Version Bilgisi Hatalidir\n",dosya_adi);
                        }
                        rewind(dptrin);
                        for(int j=0; j<5; j++)
                        {
                            char tmp;
                            tmp=fgetc(dptrin);
                            if(tmp!='\n')
                                j--;
                        }//5. satira geliyor

                        if(strstr(satir2,"VERSION")!=NULL && strstr(satir3,"ALANLAR x y z r g b")!=NULL && strstr(satir4,"NOKTALAR")!=NULL && strstr(satir5,"DATA ascii")!=NULL && nokta_sayisi==sayi && version==1)
                        {
                            // dosya satir verilerinde hata yoksa dosya icine giriliyor
                            int satir=5;
                            while(!feof(dptrin))
                            {
                                x=-1.0;
                                y=-1.0;
                                z=-1.0;
                                r=-1;
                                g=-1;
                                b=-1;
                                fgets(tmp_sayi,50,dptrin);//sayilar dosyadan tmp_sayi ya ataniyor
                                sscanf(tmp_sayi,"%f %f %f %d %d %d",&x,&y,&z,&r,&g,&b);//sayilar tmp_sayidan degiskenlere ataniyor
                                satir++;
                                if(satir==nokta_sayisi+6)//dosyanin sonundaki bosluk satiri icin yapildi. Yapilmazsa son nokta 2 kez okunuyor
                                    break;
                                if((r==-1 || g==-1 || b==-1) && (floor(x)==x || floor(y)==y || floor(z)==z))
                                {
                                    fprintf(dptrout,"%d. satirin x y z nokta bilgilerinde hata vardir\n",satir);
                                    hata=-1;
                                }
                                else if((r==-1 || g==-1 || b==-1) && (floor(x)!=x || floor(y)!=y || floor(z)!=z))
                                {
                                    fprintf(dptrout,"%d. satirin r g b nokta bilgilerinde hata vardir\n",satir);
                                    hata=-1;
                                }
                                //eger xyz den biri tam deger olarak verilirse rgb den de biri verilmezse bu kisim hatali calisiyor
                            }
                        }
                        else if(strstr(satir2,"VERSION")!=NULL && strstr(satir3,"ALANLAR x y z r g b")!=NULL && strstr(satir4,"NOKTALAR")!=NULL && strstr(satir5,"DATA binary")!=NULL && version==1)
                        {
                            int satir_sayisi=-1;
                            struct noktalar noktalar;
                            rewind(dptrin);
                            for(int j=0; j<5; j++)
                            {
                                fgets(bos_satir,50,dptrin);
                            }
                            while(!feof(dptrin))
                            {
                                fread(&noktalar,sizeof(struct noktalar),1,dptrin);
                                satir_sayisi++;
                            }
                            rewind(dptrin);
                            for(int j=0; j<5; j++)
                            {
                                fgets(bos_satir,50,dptrin);
                            }
                            struct noktalar tum_noktalar[satir_sayisi];
                            for(int j=0; j<satir_sayisi; j++)
                            {
                                fread(&tum_noktalar[j],sizeof(struct noktalar),1,dptrin);

                            }
                            rewind(dptrin);
                            for(int j=0; j<5; j++)
                            {
                                fgets(bos_satir,50,dptrin);
                            }
                            int flag=0;
                            int k=0;
                            while(k<=satir_sayisi)
                            {
                                if(satir_sayisi!=sayi)
                                {
                                    fprintf(dptrout,"%s dosyasindaki nokta sayisi uyumlu degildir\n",dosya_adi);
                                    hata=1;
                                    break;
                                }
                                if(satir_sayisi==flag)
                                {
                                    break;
                                }
                                x=-1;
                                y=-1;
                                z=-1;
                                r=-1;
                                g=-1;
                                b=-1;
                                x=tum_noktalar[k].x;
                                y=tum_noktalar[k].y;
                                z=tum_noktalar[k].z;
                                r=tum_noktalar[k].r;
                                g=tum_noktalar[k].g;
                                b=tum_noktalar[k].b;
                                if(((r==-1) || (g==-1)|| (b==-1)) && ((floor(tum_noktalar[k].x)==x) || (floor(tum_noktalar[k].y)==y) || (floor(tum_noktalar[k].z)==z)) )
                                {
                                    fprintf(dptrout,"%d. Noktanin x,y,z Verilerinde Hata Vardir\n",flag+1);
                                    hata=-1;
                                }
                                else if(((r==-1) || (g==-1) || (b==-1)) && ((floor(tum_noktalar[k].x)!=x) || (floor(tum_noktalar[k].y)!=y) || (floor(tum_noktalar[k].z)!=z)))
                                {
                                    fprintf(dptrout,"%d. Noktanin r,g,b verilerinde Hata Vardir\n",flag+1);
                                    hata=-1;
                                }
                                flag++;
                                k++;

                            }

                        }
                        else if(strstr(satir2,"VERSION")!=NULL && strstr(satir3,"ALANLAR x y z")!=NULL && strstr(satir4,"NOKTALAR")!=NULL && strstr(satir5,"DATA binary")!=NULL && version==1)
                        {
                            int satir_sayisi=-1;
                            struct noktalar2 noktalar;
                            rewind(dptrin);
                            for(int j=0; j<5; j++)
                            {
                                fgets(bos_satir,50,dptrin);
                            }
                            while(!feof(dptrin))
                            {
                                fread(&noktalar,sizeof(struct noktalar2),1,dptrin);
                                satir_sayisi++;
                            }
                            rewind(dptrin);
                            for(int j=0; j<5; j++)
                            {
                                fgets(bos_satir,50,dptrin);
                            }
                            struct noktalar2 tum_noktalar[satir_sayisi];
                            for(int j=0; j<satir_sayisi; j++)
                            {
                                fread(&tum_noktalar[j],sizeof(struct noktalar2),1,dptrin);

                            }
                            rewind(dptrin);
                            for(int j=0; j<5; j++)
                            {
                                fgets(bos_satir,50,dptrin);
                            }
                            int flag=0;
                            int k=0;
                            while(k<=satir_sayisi)
                            {
                                if(satir_sayisi!=sayi)
                                {
                                    fprintf(dptrout,"%s dosyasindaki nokta sayisi uyumlu degildir \n",dosya_adi);
                                    hata=1;
                                    break;
                                }
                                if(satir_sayisi==flag)
                                {
                                    break;
                                }
                                x=-1;
                                y=-1;
                                z=-1;
                                x=tum_noktalar[k].x;
                                y=tum_noktalar[k].y;
                                z=tum_noktalar[k].z;
                                if((x==-1) || (y==-1) || (z==-1) )
                                {
                                    fprintf(dptrout,"%d. Noktanin x,y,z Verilerinde Hata Vardir\n",flag+1);
                                    hata=-1;
                                }
                                flag++;
                                k++;
                            }
                        }
                        else if(strstr(satir2,"VERSION")!=NULL && strstr(satir3,"ALANLAR x y z")!=NULL && strstr(satir4,"NOKTALAR")!=NULL && strstr(satir5,"DATA ascii")!=NULL && nokta_sayisi==sayi && version==1)
                        {
                            int satir=5;
                            while(!feof(dptrin))
                            {
                                x=-1.0;
                                y=-1.0;
                                z=-1.0;
                                fgets(tmp_sayi,50,dptrin);
                                sscanf(tmp_sayi,"%f %f %f",&x,&y,&z);
                                satir++;
                                if(satir==nokta_sayisi+6)//dosyanin sonundaki bosluk satiri icin yapildi
                                    break;
                                if(x==-1 || y==-1 || z==-1)
                                {
                                    fprintf(dptrout,"%d. satirin x y z nokta bilgilerinde hata vardir\n",satir);
                                    hata=-1;
                                }
                            }

                        }
                        else// ilk bes satirda hata varsa ekrana bunu basar
                        {
                            hata=-1;
                            fprintf(dptrout,"%s Dosyasinda yazim hatasi vardir\n",dosya_adi);
                        }
                        if(hata==0)//hicbir sekilde hata yoksa bu calisir
                        {
                            fprintf(dptrout,"dosyada hata yoktur\n");
                        }
                    }
                    fclose(dptrin);//dosyalar kapatiliyor
                    fclose(dptrout);
                }
            }
            closedir(dizin);//dizin kapatiliyor
        }
        else if(secim==2)
        {
            DIR *dizin_out;
            DIR *dizin_in;
            struct dirent *girdi_out;
            struct dirent *girdi_in;
            //Dirent kutuphanesinin struct ve pointer i
            //Dizini actigimiz kisim
            dizin_out=opendir(".");
            if(dizin_out==NULL)
            {
                printf("Dizin Okunamadi\n");
                return(1);
            }
            // dizine girip dosya ismilerini aldigimiz kisim
            while((girdi_out=readdir(dizin_out)))
            {
                char dosya_adi_out[50]= {};
                strcpy(dosya_adi_out,(girdi_out->d_name));//dizinden dosya ismini stringe aktardigimiz kisim
                // dosyanin .nkt uzantili olup olmadigini anladigimiz kisim
                if(strstr(dosya_adi_out,"output")!=NULL  && strstr(dosya_adi_out,".nkt")!=NULL)
                {
                    FILE *dptrout;
                    dptrout=fopen(dosya_adi_out,"r+");
                    char *p;
                    p=&dosya_adi_out[7];//outout dosyasinin input dosyasinin adini almak icin yapildi adres tutuldu
                    dizin_in=opendir(".");
                    if(dizin_in==NULL)
                    {
                        printf("Dizin_in Okunamadi\n");
                        return(1);
                    }

                    while((girdi_in=readdir(dizin_in)))
                    {
                        char dosya_adi_in[50]= {};
                        strcpy(dosya_adi_in,(girdi_in->d_name));//dizinden dosya ismini stringe aktardigimiz kisim
                        if(strstr(dosya_adi_in,"output")==NULL  && strstr(dosya_adi_in,".nkt")!=NULL)
                        {
                            if(strcmp(p,dosya_adi_in)==0)
                            {
                                FILE *dptrin;
                                fgets(bos_satir,50,dptrout);//secim 1 kismi atlatilsin diye yapildi
                                fgets(bos_satir,50,dptrout);//dosyada hata olup olmadigini gormek icin yapildi

                                if(strstr(bos_satir,"dosyada hata yoktur")!=NULL)
                                {
                                    dptrin=fopen(dosya_adi_in,"r+b");
                                    for(int j=0; j<4; j++)
                                    {
                                        char tmp;
                                        tmp=fgetc(dptrin);
                                        if(tmp!='\n')
                                            j--;
                                    }
                                    //4. satira geldik
                                    fgets(bos_satir,50,dptrin);//5. satiri aldik
                                    if(strstr(bos_satir,"DATA ascii")!=NULL)//dosyanin veri tipi kontrol ediliyor
                                    {
                                        rewind(dptrin);
                                        for(int j=0; j<2; j++)
                                        {
                                            char tmp;
                                            tmp=fgetc(dptrin);
                                            if(tmp!='\n')
                                                j--;
                                        }
                                        fgets(bos_satir,50,dptrin);
                                        //3. satir alindi
                                        if(strstr(bos_satir,"ALANLAR x y z r g b")!=NULL)
                                        {
                                            fscanf(dptrin,"%s %d",bos_satir,&nokta_sayisi); //nokta sayisi alindi
                                            fgets(bos_satir,50,dptrin);
                                            fgets(bos_satir,50,dptrin);//5. satira gelindi
                                            float fark=9999999.0,maks=-1.0;
                                            struct noktalar noktalar[4];
                                            rewind(dptrin);
                                            struct noktalar tum_noktalar[nokta_sayisi];
                                            for(int j=0; j<5; j++)
                                            {
                                                char tmp;
                                                tmp=fgetc(dptrin);
                                                if(tmp!='\n')
                                                    j--;
                                            }
                                            //tum noktalar struct dizisine atiliyor
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {
                                                fscanf(dptrin,"%f %f %f %d %d %d",&x,&y,&z,&r,&g,&b);
                                                tum_noktalar[j].x=x;
                                                tum_noktalar[j].y=y;
                                                tum_noktalar[j].z=z;
                                                tum_noktalar[j].r=r;
                                                tum_noktalar[j].g=g;
                                                tum_noktalar[j].b=b;
                                            }
                                            //en yakin en uzak nokta kontrolu
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {

                                                for(int k=j+1; k<nokta_sayisi; k++)
                                                {
                                                    float denklem=sqrt(pow(tum_noktalar[j].x-tum_noktalar[k].x,2)+pow(tum_noktalar[j].y-tum_noktalar[k].y,2)+pow(tum_noktalar[j].z-tum_noktalar[k].z,2));

                                                    if(denklem<fark)
                                                    {
                                                        fark=denklem;
                                                        noktalar[0].x=tum_noktalar[j].x;
                                                        noktalar[0].y=tum_noktalar[j].y;
                                                        noktalar[0].z=tum_noktalar[j].z;
                                                        noktalar[0].r=tum_noktalar[j].r;
                                                        noktalar[0].g=tum_noktalar[j].g;
                                                        noktalar[0].b=tum_noktalar[j].b;

                                                        noktalar[1].x=tum_noktalar[k].x;
                                                        noktalar[1].y=tum_noktalar[k].y;
                                                        noktalar[1].z=tum_noktalar[k].z;
                                                        noktalar[1].r=tum_noktalar[k].r;
                                                        noktalar[1].g=tum_noktalar[k].g;
                                                        noktalar[1].b=tum_noktalar[k].b;

                                                    }
                                                    if(denklem>maks)
                                                    {
                                                        maks=denklem;
                                                        noktalar[2].x=tum_noktalar[j].x;
                                                        noktalar[2].y=tum_noktalar[j].y;
                                                        noktalar[2].z=tum_noktalar[j].z;
                                                        noktalar[2].r=tum_noktalar[j].r;
                                                        noktalar[2].g=tum_noktalar[j].g;
                                                        noktalar[2].b=tum_noktalar[j].b;

                                                        noktalar[3].x=tum_noktalar[k].x;
                                                        noktalar[3].y=tum_noktalar[k].y;
                                                        noktalar[3].z=tum_noktalar[k].z;
                                                        noktalar[3].r=tum_noktalar[k].r;
                                                        noktalar[3].g=tum_noktalar[k].g;
                                                        noktalar[3].b=tum_noktalar[k].b;

                                                    }
                                                }
                                            }

                                            fclose(dptrout);
                                            dptrout=fopen(dosya_adi_out,"a");
                                            fprintf(dptrout,"\nSecim 2\n");
                                            fprintf(dptrout,"En yakin noktalar\n");
                                            fprintf(dptrout,"%f %f %f %d %d %d\n",noktalar[0].x,noktalar[0].y,noktalar[0].z,noktalar[0].r,noktalar[0].g,noktalar[0].b);
                                            fprintf(dptrout,"%f %f %f %d %d %d\n",noktalar[1].x,noktalar[1].y,noktalar[1].z,noktalar[1].r,noktalar[1].g,noktalar[1].b);
                                            fprintf(dptrout,"En uzak noktalar\n");
                                            fprintf(dptrout,"%f %f %f %d %d %d\n",noktalar[2].x,noktalar[2].y,noktalar[2].z,noktalar[2].r,noktalar[2].g,noktalar[2].b);
                                            fprintf(dptrout,"%f %f %f %d %d %d\n",noktalar[3].x,noktalar[3].y,noktalar[3].z,noktalar[3].r,noktalar[3].g,noktalar[3].b);
                                        }

                                        else
                                        {
                                            fscanf(dptrin,"%s %d",bos_satir,&nokta_sayisi);
                                            fgets(bos_satir,50,dptrin);
                                            fgets(bos_satir,50,dptrin);
                                            float fark=9999999.0,maks=-1.0;
                                            struct noktalar noktalar[4];
                                            rewind(dptrin);
                                            struct noktalar tum_noktalar[nokta_sayisi];
                                            for(int j=0; j<5; j++)
                                            {
                                                char tmp;
                                                tmp=fgetc(dptrin);
                                                if(tmp!='\n')
                                                    j--;
                                            }
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {

                                                fscanf(dptrin,"%f %f %f",&x,&y,&z);
                                                tum_noktalar[j].x=x;
                                                tum_noktalar[j].y=y;
                                                tum_noktalar[j].z=z;

                                            }
                                            //tum noktalar struct dizisine atiliyor
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {
                                                //en yakin en uzak nokta kontrolu
                                                for(int k=j+1; k<nokta_sayisi; k++)
                                                {
                                                    float denklem=sqrt(pow(tum_noktalar[j].x-tum_noktalar[k].x,2)+pow(tum_noktalar[j].y-tum_noktalar[k].y,2)+pow(tum_noktalar[j].z-tum_noktalar[k].z,2));
                                                    if(denklem<fark)
                                                    {
                                                        fark=denklem;

                                                        noktalar[0].x=tum_noktalar[j].x;
                                                        noktalar[0].y=tum_noktalar[j].y;
                                                        noktalar[0].z=tum_noktalar[j].z;

                                                        noktalar[1].x=tum_noktalar[k].x;
                                                        noktalar[1].y=tum_noktalar[k].y;
                                                        noktalar[1].z=tum_noktalar[k].z;

                                                    }
                                                    if(denklem>maks)
                                                    {
                                                        maks=denklem;
                                                        noktalar[2].x=tum_noktalar[j].x;
                                                        noktalar[2].y=tum_noktalar[j].y;
                                                        noktalar[2].z=tum_noktalar[j].z;


                                                        noktalar[3].x=tum_noktalar[k].x;
                                                        noktalar[3].y=tum_noktalar[k].y;
                                                        noktalar[3].z=tum_noktalar[k].z;
                                                    }
                                                }
                                            }
                                            fclose(dptrout);
                                            dptrout=fopen(dosya_adi_out,"a");
                                            fprintf(dptrout,"\nSecim 2\n");
                                            fprintf(dptrout,"En Yakin Noktalar\n");
                                            fprintf(dptrout,"%f %f %f\n",noktalar[0].x,noktalar[0].y,noktalar[0].z);
                                            fprintf(dptrout,"%f %f %f\n",noktalar[1].x,noktalar[1].y,noktalar[1].z);
                                            fprintf(dptrout,"En Uzak Noktalar\n");
                                            fprintf(dptrout,"%f %f %f\n",noktalar[2].x,noktalar[2].y,noktalar[2].z);
                                            fprintf(dptrout,"%f %f %f\n",noktalar[3].x,noktalar[3].y,noktalar[3].z);

                                        }
                                    }

                                    else if(strstr(bos_satir,"DATA binary")!=NULL)
                                    {
                                        rewind(dptrin);
                                        for(int j=0; j<2; j++)
                                        {
                                            char tmp;
                                            tmp=fgetc(dptrin);
                                            if(tmp!='\n')
                                                j--;
                                        }
                                        fgets(bos_satir,50,dptrin);
                                        if(strstr(bos_satir,"ALANLAR x y z r g b")!=NULL)
                                        {
                                            fscanf(dptrin,"%s %d",bos_satir,&nokta_sayisi);
                                            fgets(bos_satir,50,dptrin);
                                            fgets(bos_satir,50,dptrin);//5. satira gelindi
                                            float fark=9999999.0,maks=-1.0;
                                            struct noktalar noktalar[4];
                                            rewind(dptrin);
                                            struct noktalar tum_noktalar[nokta_sayisi];
                                            for(int j=0; j<5; j++)
                                            {
                                                char tmp;
                                                tmp=fgetc(dptrin);
                                                if(tmp!='\n')
                                                    j--;
                                            }
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {
                                                fread(&tum_noktalar[j],sizeof(struct noktalar),1,dptrin);

                                            }
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {

                                                for(int k=j+1; k<nokta_sayisi; k++)
                                                {
                                                    float denklem=sqrt(pow(tum_noktalar[j].x-tum_noktalar[k].x,2)+pow(tum_noktalar[j].y-tum_noktalar[k].y,2)+pow(tum_noktalar[j].z-tum_noktalar[k].z,2));

                                                    if(denklem<fark)
                                                    {
                                                        fark=denklem;
                                                        noktalar[0].x=tum_noktalar[j].x;
                                                        noktalar[0].y=tum_noktalar[j].y;
                                                        noktalar[0].z=tum_noktalar[j].z;
                                                        noktalar[0].r=tum_noktalar[j].r;
                                                        noktalar[0].g=tum_noktalar[j].g;
                                                        noktalar[0].b=tum_noktalar[j].b;

                                                        noktalar[1].x=tum_noktalar[k].x;
                                                        noktalar[1].y=tum_noktalar[k].y;
                                                        noktalar[1].z=tum_noktalar[k].z;
                                                        noktalar[1].r=tum_noktalar[k].r;
                                                        noktalar[1].g=tum_noktalar[k].g;
                                                        noktalar[1].b=tum_noktalar[k].b;

                                                    }
                                                    if(denklem>maks)
                                                    {
                                                        maks=denklem;
                                                        noktalar[2].x=tum_noktalar[j].x;
                                                        noktalar[2].y=tum_noktalar[j].y;
                                                        noktalar[2].z=tum_noktalar[j].z;
                                                        noktalar[2].r=tum_noktalar[j].r;
                                                        noktalar[2].g=tum_noktalar[j].g;
                                                        noktalar[2].b=tum_noktalar[j].b;

                                                        noktalar[3].x=tum_noktalar[k].x;
                                                        noktalar[3].y=tum_noktalar[k].y;
                                                        noktalar[3].z=tum_noktalar[k].z;
                                                        noktalar[3].r=tum_noktalar[k].r;
                                                        noktalar[3].g=tum_noktalar[k].g;
                                                        noktalar[3].b=tum_noktalar[k].b;

                                                    }
                                                }
                                            }
                                            fclose(dptrout);
                                            dptrout=fopen(dosya_adi_out,"a+");
                                            fprintf(dptrout,"\nSecim 2\n");
                                            fprintf(dptrout,"En Yakin Noktalar\n");
                                            fwrite(&noktalar[0],sizeof(struct noktalar),1,dptrout);
                                            fwrite(&noktalar[1],sizeof(struct noktalar),1,dptrout);
                                            fprintf(dptrout,"\nEn Uzak Noktalar\n");
                                            fwrite(&noktalar[2],sizeof(struct noktalar),1,dptrout);
                                            fwrite(&noktalar[3],sizeof(struct noktalar),1,dptrout);


                                        }
                                        else
                                        {
                                            fscanf(dptrin,"%s %d",bos_satir,&nokta_sayisi);
                                            fgets(bos_satir,50,dptrin);
                                            fgets(bos_satir,50,dptrin);//5. satira gelindi
                                            float fark=9999999.0,maks=-1.0;
                                            struct noktalar2 noktalar[4];
                                            rewind(dptrin);
                                            struct noktalar tum_noktalar[nokta_sayisi];
                                            for(int j=0; j<5; j++)
                                            {
                                                char tmp;
                                                tmp=fgetc(dptrin);
                                                if(tmp!='\n')
                                                    j--;
                                            }
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {
                                                fread(&tum_noktalar[j],sizeof(struct noktalar2),1,dptrin);
                                            }
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {

                                                for(int k=j+1; k<nokta_sayisi; k++)
                                                {
                                                    float denklem=sqrt(pow(tum_noktalar[j].x-tum_noktalar[k].x,2)+pow(tum_noktalar[j].y-tum_noktalar[k].y,2)+pow(tum_noktalar[j].z-tum_noktalar[k].z,2));

                                                    if(denklem<fark)
                                                    {
                                                        fark=denklem;
                                                        noktalar[0].x=tum_noktalar[j].x;
                                                        noktalar[0].y=tum_noktalar[j].y;
                                                        noktalar[0].z=tum_noktalar[j].z;


                                                        noktalar[1].x=tum_noktalar[k].x;
                                                        noktalar[1].y=tum_noktalar[k].y;
                                                        noktalar[1].z=tum_noktalar[k].z;


                                                    }
                                                    if(denklem>maks)
                                                    {
                                                        maks=denklem;
                                                        noktalar[2].x=tum_noktalar[j].x;
                                                        noktalar[2].y=tum_noktalar[j].y;
                                                        noktalar[2].z=tum_noktalar[j].z;


                                                        noktalar[3].x=tum_noktalar[k].x;
                                                        noktalar[3].y=tum_noktalar[k].y;
                                                        noktalar[3].z=tum_noktalar[k].z;


                                                    }
                                                }
                                            }
                                            fclose(dptrout);
                                            dptrout=fopen(dosya_adi_out,"a+");
                                            fprintf(dptrout,"\nSecim 2\n");
                                            fprintf(dptrout,"En Yakin Noktalar\n");
                                            fwrite(&noktalar[0],sizeof(struct noktalar),1,dptrout);
                                            fwrite(&noktalar[1],sizeof(struct noktalar),1,dptrout);
                                            fprintf(dptrout,"\nEn Uzak Noktalar\n");
                                            fwrite(&noktalar[2],sizeof(struct noktalar),1,dptrout);
                                            fwrite(&noktalar[3],sizeof(struct noktalar),1,dptrout);


                                        }


                                    }
                                    fclose(dptrout);
                                    fclose(dptrin);
                                }
                            }
                        }
                    }
                }
            }
            closedir(dizin_out);
            closedir(dizin_in);
        }

        else if(secim==3)
        {
            DIR *dizin_out;
            DIR *dizin_in;
            struct dirent *girdi_out;
            struct dirent *girdi_in;
            //Dizini actigimiz kisim
            dizin_out=opendir(".");
            if(dizin_out==NULL)
            {
                printf("Dizin Okunamadi\n");
                return(1);
            }
            // dizine girip dosya ismilerini aldigimiz kisim
            while((girdi_out=readdir(dizin_out)))
            {
                char dosya_adi_out[50]= {};
                strcpy(dosya_adi_out,(girdi_out->d_name));//dizinden dosya ismini stringe aktardigimiz kisim
                // printf("%s\n",dosya_adi);
                // dosyanin .nkt uzantili olup olmadigini anladigimiz kisim
                if(strstr(dosya_adi_out,"output")!=NULL  && strstr(dosya_adi_out,".nkt")!=NULL)
                {
                    FILE *dptrout;
                    dptrout=fopen(dosya_adi_out,"a+");
                    char *p;
                    p=&dosya_adi_out[7];
                    dizin_in=opendir(".");
                    if(dizin_in==NULL)
                    {
                        printf("Dizin_in Okunamadi\n");
                        return(1);
                    }

                    while((girdi_in=readdir(dizin_in)))
                    {
                        char dosya_adi_in[50]= {};
                        strcpy(dosya_adi_in,(girdi_in->d_name));//dizinden dosya ismini stringe aktardigimiz kisim
                        if(strstr(dosya_adi_in,"output")==NULL  && strstr(dosya_adi_in,".nkt")!=NULL)
                        {
                            if(strcmp(p,dosya_adi_in)==0)
                            {
                                FILE *dptrin;
                                fgets(bos_satir,50,dptrout);
                                fgets(bos_satir,50,dptrout);
                                //printf("\n%s\n",bos_satir);
                                if(strstr(bos_satir,"dosyada hata yoktur")!=NULL)
                                {
                                    dptrin=fopen(dosya_adi_in,"r+b");
                                    for(int j=0; j<4; j++)
                                    {
                                        char tmp;
                                        tmp=fgetc(dptrin);
                                        if(tmp!='\n')
                                            j--;
                                    }
                                    fgets(bos_satir,50,dptrin);
                                    if(strstr(bos_satir,"DATA ascii")!=NULL)
                                    {
                                        rewind(dptrin);
                                        for(int j=0; j<2; j++)
                                        {
                                            char tmp;
                                            tmp=fgetc(dptrin);
                                            if(tmp!='\n')
                                                j--;
                                        }
                                        fgets(bos_satir,50,dptrin);
                                        if(strstr(bos_satir,"ALANLAR x y z r g b")!=NULL)
                                        {
                                            struct noktalar noktalar;
                                            fscanf(dptrin,"%s %d",bos_satir,&nokta_sayisi);
                                            fgets(bos_satir,50,dptrin);
                                            fgets(bos_satir,50,dptrin);
                                            double en_kucuk=9999.9,en_kucuk1=9999.9,en_kucuk2=9999.9,en_buyuk=-1.0,en_buyuk1=-1.0,en_buyuk2=-1.0;
                                            //kup algoritmasi
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {
                                                fscanf(dptrin,"%f %f %f %d %d %d",&x,&y,&z,&r,&g,&b);
                                                if(x<en_kucuk)
                                                    en_kucuk=x;
                                                else if(x>en_buyuk)
                                                    en_buyuk=x;
                                                if(y<en_kucuk1)
                                                    en_kucuk1=y;
                                                else if(y>en_buyuk1)
                                                    en_buyuk1=y;
                                                if(z<en_kucuk2)
                                                    en_kucuk2=z;
                                                else if(z>en_buyuk2)
                                                    en_buyuk2=z;
                                            }
                                            double denklem=sqrt(pow(en_buyuk-en_kucuk,2)+pow(en_buyuk1-en_kucuk1,2)+pow(en_buyuk2-en_kucuk2,2));
                                            double kenar=denklem/sqrt(3);

                                            double nokta1[3]= {en_kucuk,en_kucuk1,en_kucuk2};
                                            double nokta2[3]= {en_kucuk+kenar,en_kucuk1,en_kucuk2};
                                            double nokta3[3]= {en_kucuk,en_kucuk1+kenar,en_kucuk2};
                                            double nokta4[3]= {en_kucuk+kenar,en_kucuk1+kenar,en_kucuk2};
                                            double nokta5[3]= {en_buyuk,en_buyuk1,en_buyuk2};
                                            double nokta6[3]= {en_kucuk,en_buyuk1,en_buyuk2};
                                            double nokta7[3]= {en_buyuk,en_kucuk1,en_buyuk2};
                                            double nokta8[3]= {en_kucuk,en_kucuk1,en_buyuk2};
                                            fclose(dptrout);
                                            dptrout=fopen(dosya_adi_out,"a");
                                            fprintf(dptrout,"\nSECIM 3\n");
                                            fprintf(dptrout,"%f %f %f\n",nokta1[0],nokta1[1],nokta1[2]);
                                            fprintf(dptrout,"%f %f %f\n",nokta2[0],nokta2[1],nokta2[2]);
                                            fprintf(dptrout,"%f %f %f\n",nokta3[0],nokta3[1],nokta3[2]);
                                            fprintf(dptrout,"%f %f %f\n",nokta4[0],nokta4[1],nokta4[2]);
                                            fprintf(dptrout,"%f %f %f\n",nokta5[0],nokta5[1],nokta5[2]);
                                            fprintf(dptrout,"%f %f %f\n",nokta6[0],nokta6[1],nokta6[2]);
                                            fprintf(dptrout,"%f %f %f\n",nokta7[0],nokta7[1],nokta7[2]);
                                            fprintf(dptrout,"%f %f %f\n",nokta8[0],nokta8[1],nokta8[2]);

                                        }
                                        else
                                        {
                                            fscanf(dptrin,"%s %d",bos_satir,&nokta_sayisi);
                                            fgets(bos_satir,50,dptrin);
                                            fgets(bos_satir,50,dptrin);
                                            double en_kucuk=9999.9,en_kucuk1=9999.9,en_kucuk2=9999.9,en_buyuk=-1.0,en_buyuk1=-1.0,en_buyuk2=-1.0;
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {
                                                fscanf(dptrin,"%f %f %f",&x,&y,&z);
                                                if(x<en_kucuk)
                                                    en_kucuk=x;
                                                else if(x>en_buyuk)
                                                    en_buyuk=x;
                                                if(y<en_kucuk1)
                                                    en_kucuk1=y;
                                                else if(y>en_buyuk1)
                                                    en_buyuk1=y;
                                                if(z<en_kucuk2)
                                                    en_kucuk2=z;
                                                else if(z>en_buyuk2)
                                                    en_buyuk2=z;
                                            }
                                            double denklem=sqrt(pow(en_buyuk-en_kucuk,2)+pow(en_buyuk1-en_kucuk1,2)+pow(en_buyuk2-en_kucuk2,2));
                                            double kenar=denklem/sqrt(3);
                                            double nokta1[3]= {en_kucuk,en_kucuk1,en_kucuk2};
                                            double nokta2[3]= {en_kucuk+kenar,en_kucuk1,en_kucuk2};
                                            double nokta3[3]= {en_kucuk,en_kucuk1+kenar,en_kucuk2};
                                            double nokta4[3]= {en_kucuk+kenar,en_kucuk1+kenar,en_kucuk2};
                                            double nokta5[3]= {en_buyuk,en_buyuk1,en_buyuk2};
                                            double nokta6[3]= {en_kucuk,en_buyuk1,en_buyuk2};
                                            double nokta7[3]= {en_buyuk,en_kucuk1,en_buyuk2};
                                            double nokta8[3]= {en_kucuk,en_kucuk1,en_buyuk2};
                                            fclose(dptrout);
                                            dptrout=fopen(dosya_adi_out,"a");
                                            fprintf(dptrout,"\nSECIM 3\n");
                                            fprintf(dptrout,"%f %f %f\n",nokta1[0],nokta1[1],nokta1[2]);
                                            fprintf(dptrout,"%f %f %f\n",nokta2[0],nokta2[1],nokta2[2]);
                                            fprintf(dptrout,"%f %f %f\n",nokta3[0],nokta3[1],nokta3[2]);
                                            fprintf(dptrout,"%f %f %f\n",nokta4[0],nokta4[1],nokta4[2]);
                                            fprintf(dptrout,"%f %f %f\n",nokta5[0],nokta5[1],nokta5[2]);
                                            fprintf(dptrout,"%f %f %f\n",nokta6[0],nokta6[1],nokta6[2]);
                                            fprintf(dptrout,"%f %f %f\n",nokta7[0],nokta7[1],nokta7[2]);
                                            fprintf(dptrout,"%f %f %f\n",nokta8[0],nokta8[1],nokta8[2]);
                                        }
                                    }
                                    else if(strstr(bos_satir,"DATA binary")!=NULL)
                                    {
                                        rewind(dptrin);
                                        for(int j=0; j<2; j++)
                                        {
                                            char tmp;
                                            tmp=fgetc(dptrin);
                                            if(tmp!='\n')
                                                j--;
                                        }
                                        fgets(bos_satir,50,dptrin);
                                        if(strstr(bos_satir,"ALANLAR x y z r g b")!=NULL)
                                        {
                                            fscanf(dptrin,"%s %d",bos_satir,&nokta_sayisi);
                                            fgets(bos_satir,50,dptrin);
                                            fgets(bos_satir,50,dptrin);
                                            struct noktalar noktalar[nokta_sayisi];
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {
                                                fread(&noktalar[j],sizeof(struct noktalar),1,dptrin);
                                            }
                                            double en_kucukx=9999.9,en_kucuky=9999.9,en_kucukz=9999.9,en_buyukx=-1,en_buyuky=-1,en_buyukz=-1;
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {

                                                if(noktalar[j].x<en_kucukx)
                                                    en_kucukx=noktalar[j].x;
                                                else if(noktalar[j].x>en_buyukx)
                                                    en_buyukx=noktalar[j].x;
                                                if(noktalar[j].y<en_kucuky)
                                                    en_kucuky=noktalar[j].y;
                                                else if(noktalar[j].y>en_buyuky)
                                                    en_buyuky=noktalar[j].y;
                                                if(noktalar[j].z<en_kucukz)
                                                    en_kucukz=noktalar[j].z;
                                                else if(noktalar[j].z>en_buyukz)
                                                    en_buyukz=noktalar[j].z;
                                            }
                                            double denklem=sqrt(pow(en_buyukx-en_kucukx,2)+pow(en_buyuky-en_kucuky,2)+pow(en_buyukz-en_kucukz,2));
                                            double kenar=denklem/sqrt(3);
                                            struct noktalar2 kup[8];
                                            kup[0].x=en_kucukx;
                                            kup[0].y=en_kucuky;
                                            kup[0].z=en_kucukz;

                                            kup[1].x=en_kucukx+kenar;
                                            kup[1].y=en_kucuky;
                                            kup[1].z=en_kucukz;

                                            kup[2].x=en_kucukx;
                                            kup[2].y=en_kucuky+kenar;
                                            kup[2].z=en_kucukz;

                                            kup[3].x=en_kucukx+kenar;
                                            kup[3].y=en_kucuky+kenar;
                                            kup[3].z=en_kucukz;

                                            kup[4].x=en_buyukx;
                                            kup[4].y=en_buyuky;
                                            kup[4].z=en_buyukz;

                                            kup[5].x=en_kucukx;
                                            kup[5].y=en_buyuky;
                                            kup[5].z=en_buyukz;

                                            kup[6].x=en_buyukx;
                                            kup[6].y=en_kucuky;
                                            kup[6].z=en_buyukz;

                                            kup[7].x=en_kucukx;
                                            kup[7].y=en_kucuky;
                                            kup[7].z=en_buyukz;

                                            fclose(dptrout);
                                            dptrout=fopen(dosya_adi_out,"a+");
                                            fprintf(dptrout,"\nSECIM 3\n");
                                            for(int j=0; j<8; j++)
                                                fwrite(&kup[j],sizeof(struct noktalar2),1,dptrout);

                                        }
                                        else
                                        {
                                            fscanf(dptrin,"%s %d",bos_satir,&nokta_sayisi);
                                            fgets(bos_satir,50,dptrin);
                                            fgets(bos_satir,50,dptrin);
                                            struct noktalar2 noktalar[nokta_sayisi];
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {
                                                fread(&noktalar[j],sizeof(struct noktalar),1,dptrin);
                                            }
                                            double en_kucukx=9999.9,en_kucuky=9999.9,en_kucukz=9999.9,en_buyukx=-1,en_buyuky=-1,en_buyukz=-1;
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {

                                                if(noktalar[j].x<en_kucukx)
                                                    en_kucukx=noktalar[j].x;
                                                else if(noktalar[j].x>en_buyukx)
                                                    en_buyukx=noktalar[j].x;
                                                if(noktalar[j].y<en_kucuky)
                                                    en_kucuky=noktalar[j].y;
                                                else if(noktalar[j].y>en_buyuky)
                                                    en_buyuky=noktalar[j].y;
                                                if(noktalar[j].z<en_kucukz)
                                                    en_kucukz=noktalar[j].z;
                                                else if(noktalar[j].z>en_buyukz)
                                                    en_buyukz=noktalar[j].z;
                                            }
                                            double denklem=sqrt(pow(en_buyukx-en_kucukx,2)+pow(en_buyuky-en_kucuky,2)+pow(en_buyukz-en_kucukz,2));
                                            double kenar=denklem/sqrt(3);
                                            struct noktalar2 kup[8];
                                            kup[0].x=en_kucukx;
                                            kup[0].y=en_kucuky;
                                            kup[0].z=en_kucukz;

                                            kup[1].x=en_kucukx+kenar;
                                            kup[1].y=en_kucuky;
                                            kup[1].z=en_kucukz;

                                            kup[2].x=en_kucukx;
                                            kup[2].y=en_kucuky+kenar;
                                            kup[2].z=en_kucukz;

                                            kup[3].x=en_kucukx+kenar;
                                            kup[3].y=en_kucuky+kenar;
                                            kup[3].z=en_kucukz;

                                            kup[4].x=en_buyukx;
                                            kup[4].y=en_buyuky;
                                            kup[4].z=en_buyukz;

                                            kup[5].x=en_kucukx;
                                            kup[5].y=en_buyuky;
                                            kup[5].z=en_buyukz;

                                            kup[6].x=en_buyukx;
                                            kup[6].y=en_kucuky;
                                            kup[6].z=en_buyukz;

                                            kup[7].x=en_kucukx;
                                            kup[7].y=en_kucuky;
                                            kup[7].z=en_buyukz;

                                            fclose(dptrout);
                                            dptrout=fopen(dosya_adi_out,"a+");
                                            fprintf(dptrout,"\nSECIM 3\n");
                                            for(int j=0; j<8; j++)
                                                fwrite(&kup[j],sizeof(struct noktalar2),1,dptrout);
                                        }
                                    }
                                    fclose(dptrin);
                                    fclose(dptrout);

                                }
                            }
                        }
                    }
                }
            }
            closedir(dizin_in);
            closedir(dizin_out);
        }
        else if(secim==4)
        {
            float kure[4];
            printf("\nKurenin sira ile x y z r(yaricap) degerlerini giriniz: ");
            scanf("%f %f %f %f",&kure[0],&kure[1],&kure[2],&kure[3]);//kure verileri aliniyor
            DIR *dizin_out;
            DIR *dizin_in;
            struct dirent *girdi_out;
            struct dirent *girdi_in;
            //Dizini actigimiz kisim
            dizin_out=opendir(".");
            if(dizin_out==NULL)
            {
                printf("Dizin Okunamadi\n");
                return(1);
            }
            // dizine girip dosya ismilerini aldigimiz kisim
            while((girdi_out=readdir(dizin_out)))
            {
                char dosya_adi_out[50]= {};
                strcpy(dosya_adi_out,(girdi_out->d_name));//dizinden dosya ismini stringe aktardigimiz kisim
                // printf("%s\n",dosya_adi);
                // dosyanin .nkt uzantili olup olmadigini anladigimiz kisim
                if(strstr(dosya_adi_out,"output")!=NULL  && strstr(dosya_adi_out,".nkt")!=NULL)
                {
                    FILE *dptrout;
                    dptrout=fopen(dosya_adi_out,"a+");
                    char *p;
                    p=&dosya_adi_out[7];
                    dizin_in=opendir(".");
                    if(dizin_in==NULL)
                    {
                        printf("Dizin_in Okunamadi\n");
                        return(1);
                    }

                    while((girdi_in=readdir(dizin_in)))
                    {
                        char dosya_adi_in[50]= {};
                        strcpy(dosya_adi_in,(girdi_in->d_name));//dizinden dosya ismini stringe aktardigimiz kisim
                        if(strstr(dosya_adi_in,"output")==NULL  && strstr(dosya_adi_in,".nkt")!=NULL)
                        {
                            if(strcmp(p,dosya_adi_in)==0)
                            {
                                FILE *dptrin;
                                fgets(bos_satir,50,dptrout);
                                fgets(bos_satir,50,dptrout);
                                if(strstr(bos_satir,"dosyada hata yoktur")!=NULL)
                                {
                                    dptrin=fopen(dosya_adi_in,"r+b");
                                    for(int j=0; j<4; j++)
                                    {
                                        char tmp;
                                        tmp=fgetc(dptrin);
                                        if(tmp!='\n')
                                            j--;
                                    }
                                    fgets(bos_satir,50,dptrin);
                                    if(strstr(bos_satir,"DATA ascii")!=NULL)
                                    {
                                        rewind(dptrin);
                                        for(int j=0; j<2; j++)
                                        {
                                            char tmp;
                                            tmp=fgetc(dptrin);
                                            if(tmp!='\n')
                                                j--;
                                        }
                                        fgets(bos_satir,50,dptrin);//kure algoritmasi
                                        if(strstr(bos_satir,"ALANLAR x y z r g b")!=NULL)
                                        {
                                            fscanf(dptrin,"%s %d",bos_satir,&nokta_sayisi);
                                            fgets(bos_satir,50,dptrin);
                                            fgets(bos_satir,50,dptrin);
                                            fclose(dptrout);
                                            dptrout=fopen(dosya_adi_out,"a");
                                            fprintf(dptrout,"\nSecim 4\n");
                                            fprintf(dptrout,"cx=%f\ncy=%f\ncz=%f\ncr=%f\n",kure[0],kure[1],kure[2],kure[3]);
                                            fprintf(dptrout,"ALANLAR x y z r g b\n");
                                            fprintf(dptrout,"NOKTALAR %d\n",nokta_sayisi);
                                            fprintf(dptrout,"DATA ascii\n");
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {
                                                fgets(bos_satir,50,dptrin);
                                                sscanf(bos_satir,"%f %f %f %d %d %d",&x,&y,&z,&r,&g,&b);
                                                float denklem=sqrt(pow(kure[0]-x,2)+pow(kure[1]-y,2)+pow(kure[2]-z,2));
                                                if(denklem<=kure[3])
                                                {
                                                    fprintf(dptrout,"%f %f %f %d %d %d\n",x,y,z,r,g,b);
                                                }

                                            }
                                            rewind(dptrin);

                                        }
                                        else
                                        {
                                            fscanf(dptrin,"%s %d",bos_satir,&nokta_sayisi);
                                            fgets(bos_satir,50,dptrin);
                                            fgets(bos_satir,50,dptrin);
                                            fclose(dptrout);
                                            dptrout=fopen(dosya_adi_out,"a");
                                            fprintf(dptrout,"\nSecim 4\n");
                                            fprintf(dptrout,"cx=%f\ncy=%f\ncz=%f\ncr=%f\n",kure[0],kure[1],kure[2],kure[3]);
                                            fprintf(dptrout,"ALANLAR x y z\n");
                                            fprintf(dptrout,"NOKTALAR %d\n",nokta_sayisi);
                                            fprintf(dptrout,"DATA ascii\n");
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {
                                                fgets(bos_satir,50,dptrin);
                                                sscanf(bos_satir,"%f %f %f",&x,&y,&z);
                                                float denklem=sqrt(pow(kure[0]-x,2)+pow(kure[1]-y,2)+pow(kure[2]-z,2));
                                                if(denklem<=kure[3])
                                                {
                                                    fprintf(dptrout,"%f %f %f\n",x,y,z);
                                                }

                                            }
                                            rewind(dptrin);
                                        }
                                    }
                                    else if(strstr(bos_satir,"DATA binary")!=NULL)
                                    {
                                        rewind(dptrin);
                                        for(int j=0; j<2; j++)
                                        {
                                            char tmp;
                                            tmp=fgetc(dptrin);
                                            if(tmp!='\n')
                                                j--;
                                        }
                                        fgets(bos_satir,50,dptrin);
                                        if(strstr(bos_satir,"ALANLAR x y z r g b")!=NULL)
                                        {
                                            fscanf(dptrin,"%s %d",bos_satir,&nokta_sayisi);
                                            fgets(bos_satir,50,dptrin);
                                            fgets(bos_satir,50,dptrin);
                                            fclose(dptrout);
                                            dptrout=fopen(dosya_adi_out,"a");
                                            fprintf(dptrout,"\nSecim 4\n");
                                            fprintf(dptrout,"cx=%f\ncy=%f\ncz=%f\ncr=%f\n",kure[0],kure[1],kure[2],kure[3]);
                                            fprintf(dptrout,"ALANLAR x y z r g b\n");
                                            fprintf(dptrout,"NOKTALAR %d\n",nokta_sayisi);
                                            fprintf(dptrout,"DATA binary\n");
                                            struct noktalar noktalar[nokta_sayisi];
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {
                                                fread(&noktalar[j],sizeof(struct noktalar),1,dptrin);

                                            }
                                            rewind(dptrin);
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {
                                                float denklem=sqrt(pow(kure[0]-noktalar[j].x,2)+pow(kure[1]-noktalar[j].y,2)+pow(kure[2]-noktalar[j].z,2));
                                                if(denklem<=kure[3])
                                                {
                                                    fwrite(&noktalar[j],sizeof(struct noktalar),1,dptrout);
                                                }

                                            }


                                        }
                                        else
                                        {
                                            rewind(dptrin);
                                            for(int j=0; j<2; j++)
                                            {
                                                char tmp;
                                                tmp=fgetc(dptrin);
                                                if(tmp!='\n')
                                                    j--;
                                            }
                                            fgets(bos_satir,50,dptrin);
                                            if(strstr(bos_satir,"ALANLAR x y z")!=NULL)
                                            {
                                                fscanf(dptrin,"%s %d",bos_satir,&nokta_sayisi);
                                                fgets(bos_satir,50,dptrin);
                                                fgets(bos_satir,50,dptrin);
                                                fclose(dptrout);
                                                dptrout=fopen(dosya_adi_out,"a");
                                                fprintf(dptrout,"\nSecim 4\n");
                                                fprintf(dptrout,"cx=%f\ncy=%f\ncz=%f\ncr=%f\n",kure[0],kure[1],kure[2],kure[3]);
                                                fprintf(dptrout,"ALANLAR x y z\n");
                                                fprintf(dptrout,"NOKTALAR %d\n",nokta_sayisi);
                                                fprintf(dptrout,"DATA binary\n");
                                                struct noktalar2 noktalar[nokta_sayisi];
                                                for(int j=0; j<nokta_sayisi; j++)
                                                {
                                                    fread(&noktalar[j],sizeof(struct noktalar2),1,dptrin);

                                                }
                                                rewind(dptrin);
                                                for(int j=0; j<nokta_sayisi; j++)
                                                {
                                                    float denklem=sqrt(pow(kure[0]-noktalar[j].x,2)+pow(kure[1]-noktalar[j].y,2)+pow(kure[2]-noktalar[j].z,2));
                                                    if(denklem<=kure[3])
                                                    {
                                                        fwrite(&noktalar[j],sizeof(struct noktalar2),1,dptrout);
                                                    }

                                                }
                                            }
                                        }
                                    }
                                    fclose(dptrin);
                                    fclose(dptrout);
                                }
                            }
                        }
                    }
                }
            }
            closedir(dizin_in);
            closedir(dizin_out);
        }
        else if(secim==5)
        {
            DIR *dizin_out;
            DIR *dizin_in;
            struct dirent *girdi_out;
            struct dirent *girdi_in;
            //Dizini actigimiz kisim
            dizin_out=opendir(".");
            if(dizin_out==NULL)
            {
                printf("Dizin Okunamadi\n");
                return(1);
            }
            // dizine girip dosya ismilerini aldigimiz kisim
            while((girdi_out=readdir(dizin_out)))
            {
                char dosya_adi_out[50]= {};
                strcpy(dosya_adi_out,(girdi_out->d_name));//dizinden dosya ismini stringe aktardigimiz kisim
                // dosyanin .nkt uzantili olup olmadigini anladigimiz kisim
                if(strstr(dosya_adi_out,"output")!=NULL  && strstr(dosya_adi_out,".nkt")!=NULL)
                {
                    FILE *dptrout;
                    dptrout=fopen(dosya_adi_out,"a+");
                    char *p;
                    p=&dosya_adi_out[7];
                    dizin_in=opendir(".");
                    if(dizin_in==NULL)
                    {
                        printf("Dizin_in Okunamadi\n");
                        return(1);
                    }

                    while((girdi_in=readdir(dizin_in)))
                    {

                        char dosya_adi_in[50]= {};
                        strcpy(dosya_adi_in,(girdi_in->d_name));//dizinden dosya ismini stringe aktardigimiz kisim
                        if(strstr(dosya_adi_in,"output")==NULL  && strstr(dosya_adi_in,".nkt")!=NULL)
                        {
                            if(strcmp(p,dosya_adi_in)==0)
                            {
                                FILE *dptrin;
                                fgets(bos_satir,50,dptrout);
                                fgets(bos_satir,50,dptrout);
                                if(strstr(bos_satir,"dosyada hata yoktur")!=NULL)
                                {
                                    dptrin=fopen(dosya_adi_in,"r+b");
                                    for(int j=0; j<4; j++)
                                    {
                                        char tmp;
                                        tmp=fgetc(dptrin);
                                        if(tmp!='\n')
                                            j--;
                                    }
                                    fgets(bos_satir,50,dptrin);
                                    if(strstr(bos_satir,"DATA ascii")!=NULL)
                                    {
                                        rewind(dptrin);
                                        for(int j=0; j<2; j++)
                                        {
                                            char tmp;
                                            tmp=fgetc(dptrin);
                                            if(tmp!='\n')
                                                j--;
                                        }
                                        fgets(bos_satir,50,dptrin);
                                        if(strstr(bos_satir,"ALANLAR x y z r g b")!=NULL)
                                        {

                                            fscanf(dptrin,"%s %d",bos_satir,&nokta_sayisi);
                                            fgets(bos_satir,50,dptrin);
                                            fgets(bos_satir,50,dptrin);
                                            fclose(dptrout);
                                            dptrout=fopen(dosya_adi_out,"a");
                                            fprintf(dptrout,"\nSecim 5\n");
                                            rewind(dptrin);
                                            struct noktalar tum_noktalar[nokta_sayisi];
                                            for(int j=0; j<5; j++)
                                            {
                                                char tmp;
                                                tmp=fgetc(dptrin);
                                                if(tmp!='\n')
                                                    j--;
                                            }
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {
                                                fscanf(dptrin,"%f %f %f %d %d %d",&x,&y,&z,&r,&g,&b);
                                                tum_noktalar[j].x=x;
                                                tum_noktalar[j].y=y;
                                                tum_noktalar[j].z=z;
                                                tum_noktalar[j].r=r;
                                                tum_noktalar[j].g=g;
                                                tum_noktalar[j].b=b;
                                            }
                                            struct sonsecim deger;
                                            //fazla nokta verisi olan dosyalarda hata olustugu icin tum islem struct ile yapildi
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {

                                                for(int k=j+1; k<nokta_sayisi; k++)
                                                {
                                                    deger.denklem=sqrt(pow(tum_noktalar[j].x-tum_noktalar[k].x,2)+pow(tum_noktalar[j].y-tum_noktalar[k].y,2)+pow(tum_noktalar[j].z-tum_noktalar[k].z,2));
                                                    deger.nokta_uzaklik+=deger.denklem;
                                                }
                                            }
                                            //noktalarin birbirine olan uzakliklarinin ortalamasi
                                            deger.toplam_kiyaslama=((long double)nokta_sayisi*((long double)nokta_sayisi-1))/2;
                                            deger.nokta_uzaklik/=deger.toplam_kiyaslama;
                                            fprintf(dptrout,"%lf\n",deger.nokta_uzaklik);
                                        }
                                        else
                                        {

                                            fscanf(dptrin,"%s %d",bos_satir,&nokta_sayisi);
                                            fgets(bos_satir,50,dptrin);
                                            fgets(bos_satir,50,dptrin);
                                            fclose(dptrout);
                                            dptrout=fopen(dosya_adi_out,"a");
                                            fprintf(dptrout,"\nSecim 5\n");
                                            rewind(dptrin);
                                            struct noktalar tum_noktalar[nokta_sayisi];
                                            for(int j=0; j<5; j++)
                                            {
                                                char tmp;
                                                tmp=fgetc(dptrin);
                                                if(tmp!='\n')
                                                    j--;
                                            }
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {

                                                fscanf(dptrin,"%f %f %f",&x,&y,&z);
                                                tum_noktalar[j].x=x;
                                                tum_noktalar[j].y=y;
                                                tum_noktalar[j].z=z;

                                            }
                                            struct sonsecim deger;
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {

                                                for(int k=j+1; k<nokta_sayisi; k++)
                                                {
                                                    deger.denklem=sqrt(pow(tum_noktalar[j].x-tum_noktalar[k].x,2)+pow(tum_noktalar[j].y-tum_noktalar[k].y,2)+pow(tum_noktalar[j].z-tum_noktalar[k].z,2));
                                                    deger.nokta_uzaklik+=deger.denklem;
                                                    deger.denklem=0;
                                                }

                                            }
                                            deger.toplam_kiyaslama=((long double)nokta_sayisi*((long double)nokta_sayisi-1))/2;
                                            //printf("%f\n",deger.toplam_kiyaslama);
                                            deger.nokta_uzaklik/=deger.toplam_kiyaslama;
                                            //printf("%f\n",deger.nokta_uzaklik);
                                            fprintf(dptrout,"%lf\n",deger.nokta_uzaklik);
                                        }
                                    }
                                    else if(strstr(bos_satir,"DATA binary")!=NULL)
                                    {

                                        rewind(dptrin);
                                        for(int j=0; j<2; j++)
                                        {
                                            char tmp;
                                            tmp=fgetc(dptrin);
                                            if(tmp!='\n')
                                                j--;
                                        }
                                        fgets(bos_satir,50,dptrin);
                                        if(strstr(bos_satir,"ALANLAR x y z r g b")!=NULL)
                                        {

                                            fscanf(dptrin,"%s %d",bos_satir,&nokta_sayisi);
                                            fgets(bos_satir,50,dptrin);
                                            fgets(bos_satir,50,dptrin);
                                            fclose(dptrout);
                                            dptrout=fopen(dosya_adi_out,"a");
                                            fprintf(dptrout,"\nSecim 5\n");
                                            rewind(dptrin);
                                            struct noktalar tum_noktalar[nokta_sayisi];
                                            for(int j=0; j<5; j++)
                                            {
                                                char tmp;
                                                tmp=fgetc(dptrin);
                                                if(tmp!='\n')
                                                    j--;
                                            }
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {
                                                fread(&tum_noktalar[j],sizeof(struct noktalar),1,dptrin);
                                            }
                                            struct sonsecim deger;
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {

                                                for(int k=j+1; k<nokta_sayisi; k++)
                                                {
                                                    deger.denklem=sqrt(pow(tum_noktalar[j].x-tum_noktalar[k].x,2)+pow(tum_noktalar[j].y-tum_noktalar[k].y,2)+pow(tum_noktalar[j].z-tum_noktalar[k].z,2));
                                                    deger.nokta_uzaklik+=deger.denklem;
                                                    // printf("%lf\n",deger.nokta_uzaklik);
                                                }
                                            }
                                            deger.toplam_kiyaslama=((long double)nokta_sayisi*((long double)nokta_sayisi-1))/2;
                                            deger.nokta_uzaklik/=deger.toplam_kiyaslama;
                                            fwrite(&deger.nokta_uzaklik,sizeof(struct sonsecim),1,dptrout);


                                        }
                                        else
                                        {

                                            fscanf(dptrin,"%s %d",bos_satir,&nokta_sayisi);
                                            fgets(bos_satir,50,dptrin);
                                            fgets(bos_satir,50,dptrin);
                                            fclose(dptrout);
                                            dptrout=fopen(dosya_adi_out,"a");
                                            fprintf(dptrout,"\nSecim 5\n");
                                            rewind(dptrin);
                                            struct noktalar2 tum_noktalar[nokta_sayisi];
                                            for(int j=0; j<5; j++)
                                            {
                                                char tmp;
                                                tmp=fgetc(dptrin);
                                                if(tmp!='\n')
                                                    j--;
                                            }
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {
                                                fread(&tum_noktalar[j],sizeof(struct noktalar2),1,dptrin);
                                            }
                                            struct sonsecim deger;
                                            for(int j=0; j<nokta_sayisi; j++)
                                            {

                                                for(int k=j+1; k<nokta_sayisi; k++)
                                                {
                                                    deger.denklem=sqrt(pow(tum_noktalar[j].x-tum_noktalar[k].x,2)+pow(tum_noktalar[j].y-tum_noktalar[k].y,2)+pow(tum_noktalar[j].z-tum_noktalar[k].z,2));
                                                    deger.nokta_uzaklik+=deger.denklem;

                                                }
                                            }
                                            deger.toplam_kiyaslama=((long double)nokta_sayisi*((long double)nokta_sayisi-1))/2;
                                            deger.nokta_uzaklik/=deger.toplam_kiyaslama;
                                            fwrite(&deger.nokta_uzaklik,sizeof(struct sonsecim),1,dptrout);
                                        }

                                    }
                                    fclose(dptrin);
                                    fclose(dptrout);


                                }
                            }
                        }
                    }
                }
            }
            closedir(dizin_in);
            closedir(dizin_out);
        }

        else//hatali secimde ekrana hatayi bastirmak icin yapildi
        {
            printf("Hatali Secim Yaptiniz!\n");
            i=i-1;
        }

    }

    return 0;
}
