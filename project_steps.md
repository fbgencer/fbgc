# Project Steps

- [x] Parser ve Grammar tablosu oluşturma otomatik hâle getirilecek


​	Semboller yazılırken önce c fonksiyonu mu değil mi diye bakılıyor. Bunu değiştirmek gerek.

- [x] c-fonksiyonlarını önceden tanıyoruz. Böylece kullanıcı yeni bir fonksiyon tanımlayıp c fonks 

  dokunamıyor. Bunun sebebi ise bizim tarafımızdan doğrudan fbgc kütüphanesi olarak yazılan kodların hızlıca parser tarafından tanınması ve gecikme yaşatmaması
  
  



# TODO

- [ ] x = 1,2,3 şeklinde girdileri doğrudan tuple olarak anlamalıyız

  Bu durumda gramer tablosu düzeltilince yapılması gereken testler var

  x = print(1,2,3,5,6)

  x = print 1,2,3,5,6

  yukarıdakilerin her ikisi de fonksiyon çağırmada çalışıyor

  x = y 1,2,3,5,6 bu durumda  [M]->{1}ID{G<1>}{2}{3}{5}{6}{BUILD_TUPLE(5)}{=}<->[T] şeklinde çıktı alıyoruz. Identifier unbalanced liste görünce çıkmaması gerekiyor.

- [x] "load" fonksiyonu c kütüphanelerini taramak için kullanılmalı

  - Bazı kütüphaneler io,math gibi default olarak import edilsin.
  - Diğer kütüphaneler load("module_name","expected functions") şeklinde alınabilir.

- [x] Hatalar anlamlı bir biçimde ekrana bastırılsın



## OPTIMIZATIONS

- While döngüsünde kontrol ettiğimiz değişken while tarafından bir değişkene tutulmalı ve sürekli obje değiştirmek yerine bu değişkene atanmalı böylece her işlemde fazla obje üretmek yerine aynı objenin değerini değiştirmek hızını artıracak.

  ```matlab
  tm = load('time','*') # loads the whole time module
  tm = load('math','sin') #loads only sin function from math library
  
  big_number = 50
  
  y = 0
  i = 0
  
  t = tic()
  while i < big_number
  	y = i
  	i += 1
  end
  dif = toc(t)
  
  print("While :%f sec"%dif)
  ```

  

- Range objesi optimize edilmeli, for döngüsü range ile aşırı yavaş çalışıyor 