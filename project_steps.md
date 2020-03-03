# Project Steps

- [x] Parser ve Grammar tablosu oluşturma otomatik hâle getirilecek


​	Semboller yazılırken önce c fonksiyonu mu değil mi diye bakılıyor. Bunu değiştirmek gerek.

- [x] c-fonksiyonlarını önceden tanıyoruz. Böylece kullanıcı yeni bir fonksiyon tanımlayıp c fonks 

  dokunamıyor. Bunun sebebi ise bizim tarafımızdan doğrudan fbgc kütüphanesi olarak yazılan kodların hızlıca parser tarafından tanınması ve gecikme yaşatmaması
  
  



# TODO

- [x] x = 1,2,3 şeklinde girdileri doğrudan tuple olarak anlamalıyız

  Bu durumda gramer tablosu düzeltilince yapılması gereken testler var

  x = print(1,2,3,5,6)

  x = print 1,2,3,5,6

  yukarıdakilerin her ikisi de fonksiyon çağırmada çalışıyor

  x = y 1,2,3,5,6 bu durumda  [M]->{1}ID{G<1>}{2}{3}{5}{6}{BUILD_TUPLE(5)}{=}<->[T] şeklinde çıktı alıyoruz. Identifier unbalanced liste görünce çıkmaması gerekiyor.

- [x] "load" fonksiyonu c kütüphanelerini taramak için kullanılmalı

  - Bazı kütüphaneler io,math gibi default olarak import edilsin.
  - Diğer kütüphaneler load("module_name","expected functions") şeklinde alınabilir.

- [x] Hatalar anlamlı bir biçimde ekrana bastırılsın



## Eksikler

- Interpreter hata ile karşılaştığında hangi satırda hata olduğunu söylemiyor. Hangi satırda bunun bilgisi yok. Bir line obje yapılarak bu daha kullanıcı dostu hâle getirilebilir.

- Parser fonksiyonu optimize edilmeli

- precedence table her durum için optimize edilmeli, özellikle else edurumunda assert at ve hangileri buna takılıyor gör

- assign_subscript'i bir assignment operatörü gibi tokens.h içinde yerleştir

- Gramer tablosu küçültülmeli

- Linked list yapısı ve objelerde bulunun next pointerları kaldırılmalı.

- Token generator ve gramer tablosu aynı fonksiyonda halledilmeli

- ~~Gereksiz tokenlar silinmeli~~ Kullanılmayan 2 token var LBRACE ve RBRACE

- Hata basılırken dosya adı çıkmalı

- Modüle'den ya da Cstruct objesinden member çağırılma koşulları test edilmeli. Cstruct'tan fonksiyon çağırabiliyoruz ama variable çağırılması mümkün mü ? Bu durumda math modülündeki pi ve e'nin her koşulda çalışması sağlanabilir.

- Matrislerde elementwise işlem yapan operatorler yok

- Semicolon olursa debug modu gibi yazmalıyız yani line hakkında bilgimiz olmalı

- Dökümantasyon eksikliği kodlar içine yazılacak yorum satırlarını okuyacak bir program ile giderilmeli.

- Identifier objelerinin kullanılmaları muhtemelen hata barındırıyor ve interpreter içinde çok fazla if koşuluna maruz kalıyor.

- Garbage collection modülü yok

- Block comment eklenmeli

- C kütüphanelerindeki her dosyaya ayrıca test dosyası eklenmeli kütüphanelerin kullanımı göstermek için

- ~~`x[0](3,4)` şeklindeki girdiler fonksiyon olarak çağrılamıyor çünkü burada~~ 

  ~~0,1,3,4,2,x,fun_call şeklinde bir tree çıkıyor ancak 3,4,2,0,1,x,fun_call olmalıydı.~~ Fonksiyon isimleri artık parametrelerden önce listeye atılıyor.

- gtk ile user interface kütüphanesi









## OPTIMIZATIONS

- While döngüsünde kontrol ettiğimiz değişken while tarafından bir değişkene tutulmalı ve sürekli obje değiştirmek yerine bu değişkene atanmalı böylece her işlemde fazla obje üretmek yerine aynı objenin değerini değiştirmek hızını artıracak. (bu mümkün değil)

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

  

- Range objesi optimize edilmeli, for döngüsü range ile aşırı yavaş çalışıyor. For döngüsü optimize edildi. Fazladan koyulan bir iterator ile yeni obje üretmek yerine onun değeri yerinde değiştiriliyor.



## TEST CODES

`

```matlab

fact = fun(n)
	if n == 1
   		return n
	elif n < 1
   		return ("NA")
	else
   		return n*recur_factorial(n-1)
	end
	
	print (recur_factorial(int(num)))
end

#x = -3
#y = (fun(a); y = 10; print(y); return 10*a; end,3,4)


#
#x(10)
#x = (1,2,3)
#x[1]
#y[0](5)
x('a','b')
#x.f('33')
#x = 6+1
#x[10,20,30] = 'FLOL' 
#x.y.z.e.f.g[3]; x('a')
#y = x[0]
#x.y[20]=3
#x.y = 3

```

