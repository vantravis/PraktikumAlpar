  GNU nano 2.2.6                                                                                                                                                                                                            File: ujian1.c                                                                                                                                                                                                                                                                                                                                                                                                                               



#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 100
#define Master 0
#define pesanMaster 1          
#define pesanPekerja 2         

double   waktuAwal,waktuAkhir,
         MatrixA[N][N],           
         MatrixB[N][N],           
         Hasil[N][N];



int   jumlahTugas,
   noTugas,
   jumlahPekerja,           
   sumber,
   tujuan,
   tPesan,
   baris,
   rtBaris, extra, offset, 
   i, j, k, ket;

double HitungWaktu(double waktuAwal,double waktuAKhir){

   return waktuAKhir-waktuAwal;

}


int main (int argc, char *argv[])
{

   MPI_Status status;

   MPI_Init(&argc,&argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&noTugas);
   MPI_Comm_size(MPI_COMM_WORLD,&jumlahTugas);

   //Cek Jumlah Porcessor
  if (jumlahTugas < 2 ) {

  printf(" Membutuhkan Jumlah Processor lebih\n");
  MPI_Abort(MPI_COMM_WORLD, ket);
  exit(1);
  
  }

  jumlahPekerja = jumlahTugas-1;


/**************************** Tugas Master ************************************/
   if (noTugas == Master){

      printf(" Mulai dengan tugas . %d \n",jumlahTugas);
      printf(" Input Random Angka Ke Dalam Array...\n");
      // Memasukkan Angka Random Ke Matrix A
      for (i=0; i<N; i++){
         for (j=0; j<N; j++){
            MatrixA[i][j]= rand()%10;
         }
      }
      // Memasukkan Angka Random Ke Matrix B
      for (i=0; i<N; i++){
         for (j=0; j<N; j++){
            MatrixB[i][j]= rand()%10;
         }
      }

      // Kirim Data Matrix Ke Pekerja
      rtBaris = N/jumlahPekerja;
      extra = N%jumlahPekerja;
      offset = 0;
      tPesan = pesanMaster;

      for (tujuan=1; tujuan<=jumlahPekerja; tujuan++){

         baris = (tujuan <= extra) ? rtBaris+1 : rtBaris;       
         printf(" Kirim %d baris untuk tugas %d pada offset =%d\n",baris,tujuan,offset);
         MPI_Send(&offset, 1, MPI_INT, tujuan, tPesan, MPI_COMM_WORLD);
         MPI_Send(&baris, 1, MPI_INT, tujuan, tPesan, MPI_COMM_WORLD);
         MPI_Send(&MatrixA[offset][0], baris*N, MPI_DOUBLE, tujuan, tPesan,
                   MPI_COMM_WORLD);
         MPI_Send(&MatrixB, N*N, MPI_DOUBLE, tujuan, tPesan, MPI_COMM_WORLD);
         offset = offset + baris;

      }

     // Terima status dari pekerja
      tPesan = pesanPekerja;
      for (i=1; i<=jumlahPekerja; i++){

         sumber = i;
         MPI_Recv(&offset, 1, MPI_INT, sumber, tPesan, MPI_COMM_WORLD, &status);
         MPI_Recv(&baris, 1, MPI_INT, sumber, tPesan, MPI_COMM_WORLD, &status);
         MPI_Recv(&Hasil[offset][0], baris*N, MPI_DOUBLE, sumber, tPesan, 
                  MPI_COMM_WORLD, &status);
         printf(" Menerima Hasil Dari Tugas %d\n",sumber);

      }

      // Mencetak Hasil Perkalian Matrix
      printf("_____________________________________________\n");
      printf(" Hasil Perkalian Matrix:\n");
      for (i=0; i<N; i++)
      {
         printf("\n"); 
         for (j=0; j<N; j++){ 
            printf(" %6.2f", Hasil[i][j]);
         }
      }
      printf("\n____________________________________________\n");


   }


/**************************** Tugas Pekerja ************************************/
   if (noTugas > Master){

      //Terima Perintah Dari Master
      tPesan = pesanMaster;
      MPI_Recv(&offset, 1, MPI_INT, Master, tPesan, MPI_COMM_WORLD, &status);
      MPI_Recv(&baris, 1, MPI_INT, Master, tPesan, MPI_COMM_WORLD, &status);
      MPI_Recv(&MatrixA, baris*N, MPI_DOUBLE, Master, tPesan, MPI_COMM_WORLD, &status);
      MPI_Recv(&MatrixB, N*N, MPI_DOUBLE, Master, tPesan, MPI_COMM_WORLD, &status);

      //Proses perkalian Matrix
      waktuAwal=MPI_Wtime();
      for (k=0; k<N; k++){
         for (i=0; i<baris; i++){
            Hasil[i][k] = 0.0;
            for (j=0; j<N; j++){
               Hasil[i][k] = Hasil[i][k] + MatrixA[i][j] * MatrixB[j][k];
            }
         }
      }
      waktuAkhir=MPI_Wtime();

      //Kirim Status Kerja Ke Master
      tPesan = pesanPekerja;

      MPI_Send(&offset, 1, MPI_INT, Master, tPesan, MPI_COMM_WORLD);
      MPI_Send(&baris, 1, MPI_INT, Master, tPesan, MPI_COMM_WORLD);
      MPI_Send(&Hasil, baris*N, MPI_DOUBLE, Master, tPesan, MPI_COMM_WORLD);
   }

   if((waktuAkhir-waktuAwal)!=0.0){

      printf("Waktu Komputasi Processor : %f",HitungWaktu(waktuAwal,waktuAkhir));

   }

   MPI_Finalize();
}


