---
lang: ar
dir: rtl
title: "Put the title"
author: "Author name"
date: "Publication date"
---

<body dir="rtl" style="text-align: right">
<font size="3">

# <center style="color: green"> **آزمایشگاه سیستم‌های عامل** </center><br/>

## <center style="color: yellow"> **پروژه چهارم** </center>

## <center> حسین بیاتی </center><br/>

<hr style="border-bottom: solid 1px yellow"><br/>

### <center><strong style="color: green; font-size: 24px;"> همگام سازی در xv6 </strong></center><br/>

<ul style="list-style-type: none">
    <li>
        <h2>
            <center><strong style="color: yellow">
                ۱
            </strong></center>
        </h2>
            <br/>
            <h3>
                در هسته xv6 قفل کردن با بهره گیری از ترکیبی از قفل‌های چرخشی و مدیریت وقفه ممکن شده است. توابع `aquire` و `release` یک پیاده سازی ساده از قفل‌های چرخشی در اختیار می‌گذارند که انتظار مشغول را برای aquire و release کردن قفل ها به کار می‌برد.
                <br/><br/>
                <ul>
                    <li>
                        <strong style="color: yellow">pushcli:</strong> &nbsp; این تابع مسئول نا فعال کردن وقفه‌ها با پاک کردن پرچم وقفه و ذخیره‌ی ارزش پیشین آن است. pushcli از دستور cli (clear interrupt) برای نافعال سازی وقفه‌ها بهره گرفته و وضع پیشین وقفه را باز می‌گرداند.                
                    </li><br/>
                    <li>
                        <strong style="color: yellow">popcli:</strong> &nbsp; این تابع وضع وقفه را به ارزشی که بدست pushcli ذخیره شده بود باز می‌گرداند. popcli از دستور sti (set interrupts) کمک می‌گیرد تا اگر وضعیت پیشین گویای این است که وقفه فعال بوده، آن را فعال کند.   
                    </li>
                </ul><br/>
                تفاوت اصلی میان دستورات cli و sti و توابع pushcli و popcli این است که دوتای آخر به ما اجازه‌ی ذخیره سازی و بازیابی وضعیت پیشین وقفه را می‌دهند. این از آن رو مهم است که وقتی یک قفل را aquire می‌کنیم، نیاز داریم بدانیم که وقفه‌ها در اصل فعال یا نافعال بوده‌اند تا بتوانیم وضعیت درست را هنگام آزاد کردن یک قفل بازیابیم. با ذخیره سازی و بازیابی یک وضعیت وقفه، هسته‌ی xv6 اطمینان می‌گیرد که اگر وقفه‌هایی پیش از aquire کردن یک قفل فعال بوده‌اند، آنها را نافعال رها نمی‌کند.
            </h3>
            <br/><hr style="border-bottom: solid 1px yellow"><br/>
    </li>
    <li>
        <h2>
            <center><strong style="color: yellow">
                ۲
            </strong></center>
        </h2>
            <br/>
            <h3>
                در هسته‌ی xv6 توابع acquiresleep و releasesleep یک راه جایگزین برای قفل کردن در مقایسه با قفل‌های گردشی ارائه می‌دهد. این توابع مشکل انتظار مشغول را حل کرده و تعامل میان پردازه‌ها را با پیاده سازی مفاهیم sleep-wakeup ممکن می‌کنند.
                <br/><br/>
                وقتی یک پردازه برای گرفتن یک قفل با بهره گیری از acquiresleep تلاش می‌کند و با خبر می‌شود که قفل در دست پردازه‌ی دیگریست، به طول پیوسته دچار گردش یا انتظار مشغول نمی‌شود. در عوض پردازه به خواب رفته، پردازنده را آزاد می‌کند و به دیگر پردازه‌ها اجازه‌ی اجرا می‌دهد. این عملیاتِ خواب، پردازه را در یک وضع مسدود قرار می‌دهد تا قفل آزاد شود.
                <br/><br/>
                پردازه‌ی دارای قفل، با آزاد کردن آن به کمک releasesleep بررسی می‌کند اگر پردازه‌های دیگری در انتظار این قفل هستند. اگر بله، یکی از پردازه‌های در خواب را بیدار می‌کند تا اجرا را سر بگیرد. این تعامل میان پردازه‌ها، جایی که یک پردازه یک قفل را می‌گیرد و دیگر پردازه‌ی منتظر قفلی بیدار می‌شود، هماهنگ سازی میان پردازگان را ممکن می‌سازد.
                <br/><br/>
                اگر قفل های چرخشی در مسئله‌ی تولیدگر - مصرف‌گر به کار می‌رفتند، یک مصرف کننده ممکن است مکررا در حال بررسی بافر برای داده، در حالی که خالیست، گردش کند. این می‌توانست به انتظار مشغول و هدر رفت پردازنده بیانجامد. در این مورد، بهره گیری از مفاهیم sleep-wakeup فراهم شده بدست aquiresleep و releasesleep کارآتر است. پردازه‌ی مصرف گر می‌تواند تا بافر خالیست بخوابد و وقتی داده در دسترس است بدست پردازه‌ی تولیدگر بیدار شود.
                <br/><br/>
                با مجاز سازی پردازه‌ها به خواب و بیداری در با رفتاری مهار پذیر، aquiresleep و releasesleep راهی کارآتر و هماهنگ‌تر برای ارتباطات میان پردازه‌ای فراهم می‌آورند. به خصوص در رویه‌هایی مانند تولیدگر - مصرف‌گر که انتظار مشغول دلخواه ما نیست.
            </h3>
            <br/><hr style="border-bottom: solid 1px yellow"><br/>
    </li>
    <li>
        <h2>
            <center><strong style="color: yellow">
                ۳
            </strong></center>
        </h2>
            <br/>
            <h3>
                در پردازنده‌ی xv6 یک پردازه می‌تواند در اوضاع گوناگونی در طول عمرش وجود داشته باشد. این وضعیت‌ها نماینده‌ی مراحل مختلف اجرا و مدیریت هستند. وضعیت‌های اصلی پردازه در xv6 بدین شرحند:
                <br/><br/>
                <ul>
                    <li>
                        <strong style="color: yellow">Unused:</strong> &nbsp; این وضعیت گویای این است که مدخل پردازه در جدول پردازه در استفاده یا اختصاص یافته به پردازه‌ای نیست.          
                    </li><br/>
                    <li>
                        <strong style="color: yellow">Embryo:</strong> &nbsp; در این وضعیت پردازه مقدار دهی شده. داده ساختارهای واجب اختصاص داده شده و پردازه در آماده سازی برای اجراست.
                    </li><br/>
                    <li>
                        <strong style="color: yellow">Sleeping:</strong> &nbsp; یک پردازه‌ی خفته منتظر وقوع یک رخداد یا شرایط است. ممکن است که منتظر گذر مقدار خاصی زمان برای تکمیل عملیات I/O، یا پردازه‌ای دیگر که با بهره گیری از ساز و کار sleep-wakeup بیدارش کند باشد.
                    </li><br/>
                    <li>
                        <strong style="color: yellow">Runnable:</strong> &nbsp; یک پردازه‌ی اجرا پذیر آماده‌ی برای اجرا و منتظر برای زمان بندی شدن بر پردازنده است. این پردازه می‌تواند در انتظار زمانبند برای اختصاص زمانی از پردازنده به آن باشد.
                    </li><br/>
                    <li>
                        <strong style="color: yellow">Running:</strong> &nbsp; یک پردازه‌ی در اجرا پردازه‌ایست که هم اکنون در حال اجرا بدست پردازنده است. در هر زمان، در یک تک پردازنده، تنها یک پردازه می‌تواند در وضع در اجرا باشد.
                    </li><br/>
                    <li>
                        <strong style="color: yellow">Zombie:</strong> &nbsp; وقتی یک پردازه اجرای خود را تمام می‌کند، وارد وضع زامبی می‌شود. در این وضعیت، پردازه از نظر فنی پایان یافته است، اما وضعیت خروج و اطلاعات دیگر آن هنوز از سوی والدش مورد نیاز است. پردازه در وضعیت زامبی باقی می‌ماند تا والدش برای دریافت اطلاعات خروجش فراخوانی سیستمی wait را صدا کند و پس از آن از جدول پردازگان حذف می‌شود.
                    </li><br/>
                    <li>
                        <strong style="color: yellow">Dead:</strong> &nbsp; وقتی پردازه کاملا بدست والدش پاکسازی می شود، وارد وضعیت مرده و مدخل پردازه‌اش آزاد می‌شود و برای باز استفاده در دسترس می‌شود.
                </ul><br/>
                تابع sched در xv6 مسئول زمان بندیست. این تابع قلب زمان بند است و مشخص می‌کند چه پردازه‌ای زمان CPU دریافت کند و کِی.
                تابع sched() پردازه‌ی اجرا پذیر بعدی را بر پایه‌ی یک الگوریتم زمان بندی انتخاب می‌کند (در xv6 یک round-robin ساده به کار رفته است) و تغییر متن‌های واجب را انجام می‌دهد تا پردازه‌ها را جایگزین کند.
                این تابع جابجایی‌های وضعیت‌های گوناگون پردازه مانند انتقال یک پردازه‌ی در اجرا به وضع اجرا پذیر یا به خواب بردن یک پردازه را مدیریت می‌کند. تابع sched() بدست بخش‌های گوناگونی از هسته صدا می‌شود؛ شامل زمانی که یک پردازه به داوطلبانه پردازنده را آزاد می‌کند.
                وقتی یک پردازه به خواب می‌رود، یا زمانی که یک وقفه رخ می‌دهد و یک پردازه‌ی تازه نیاز به زمان بندی شدن دارد.
            </h3>
            <br/><hr style="border-bottom: solid 1px yellow"><br/>
    </li>
    <li>
        <h2>
            <center><strong style="color: yellow">
                ۴
            </strong></center>
        </h2>
            <br/>
            <h3>
                با کمک فیلد pid در sleeplock که در آن id پردازه ذخیره می‌شود، بررسی می‌کنیم که آیدی ذخیره شده با آیدی پردازه کنونی برابر است یا خیر.<br/><br/>
                کد بدین شکل تغییر می‌کند که پس از خط <br/>
                <strong style="color: yellow">;acquire(&lk->lk)</strong><br/>
                در تابع releasesleep, شرط <br/>  <strong style="color: yellow">(myproc()->pid == lk -> pid) </strong><br/>بررسی می‌شود. <br/><br/>
                در سیستم عامل لینوکس، قفلی که تنها به مالک خود اجازه‌ی آزاد سازی را می‌دهد، mutex (mutual exclusion). 
                یک mutex داده‌ساختاری اولیه برای هماهنگ سازی است که دسترسی انحصاری به یک منبع مشترک را فراهم آورده و مطمئن می‌شود که هر زمان تنها یک نخ یا پردازه می‌تواند قفل را دارا باشد. زمانی که یک نخ یا پردازه با موفقیت یک قفل mutex را دست می‌گیرد، بدل به مالک قفل شده و تنها مالک توانا به رهاسازی قفل است.
                در لینوکس mutex ها با بهره از نود داده‌ی `pthread_mutex_t` پیاده شده‌اند که بخشی از کتابخانه‌ی ریسمان های POSIX است.
            </h3>
            <br/><hr style="border-bottom: solid 1px yellow"><br/>
    </li>
    <li>
        <h2>
            <center><strong style="color: yellow">
                ۵
            </strong></center>
        </h2>
            <br/>
            <h3>
                حافظه تراکنشی فناوری ایست که برنامه نویسی موازی را، با مجاز کردن چند نخ برای اجرای همروند انتقالات بی نیاز به قفل‌های صریح، ساده‌تر می‌کند. <br/>
                حذف قفل روشیست که در پیاده سازی TSX اینتل از TM بکار رفته است.<br/>
                حذف قفل کارآیی را با از بین بردن سربار اکتساب / رهاساخت قفل‌ها وقتی تعارضی وجود ندارد بالا می‌برد و همروندی را افزایش، و رقابت را کاهش می‌دهد.
            </h3>
            <br/><hr style="border-bottom: solid 1px yellow"><br/>
    </li>
</ul>

### <center><strong style="color: green; font-size: 24px;"> شبیه سازی مسئله readers-writers readers-priority </strong></center><br/>
<ul>
    <li></li>
    <li></li>
    <li></li>
</ul>
</font>
</body>

