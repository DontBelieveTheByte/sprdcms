#SprdCMS

Content management for your [Spreadshirt](http://www.spreadshirt.com/) shop.

Copyright (c) J-F B 2013

Contact Info. <ever.livin.never.fearful@gmail.com>

Published under the GPLv3 license.

[Introduction video playlist](http://www.youtube.com/playlist?list=PLhYuGjlMiCxi4KmRhJcR2NsoLU_c3nHh6)

---

##Requirements

- MySQL with InnoDB storage engine.
    - MyISAM could possibly work but does not support transactions and could corrupt the database.

- PHP 5 (tested on 5.2, 5.3 and 5.4).

- A Modern browser with Javascript enabled for the admin pannel.
    - Tested on the latest Chrome and Firefox.

---

##Features

- Import Spreadshirt articles from a CSV file.

- Organize your shop however you like.
    - Create categories.
    - Organize articles within those categories.

- Store front is easy to customize and extend.
    - Fully themable with [Twitter Bootstrap](http://twitter.github.com/bootstrap/)
    - Uses jQuery and jQueryUI.
    - Nice icons with [Font Awesome](http://fortawesome.github.com/Font-Awesome/).

- Clean URLs.
    - Site becomes more user friendly.
    - Better SEO.

- Can run along your any of your existing website within a subdirectory.

- Caching can be turned on to improve performance.

- Full API documentation provided.

---

##Quick Installation
1. Copy all the content inside of the /cms folder into your web directory.
    -SprdCMS can also coexist in a subdirectory of an existing website.

2. Create a MySQL database.

3. Fill out the installer form.You will need information about your Spreadshirt account :
    - Your shop name.
    - Your shop ID.
    - Your shop domain. Did you register with spreadshirt.com or spreadshirt.net?
4. Start administering your store.
    - Create categories
    - Add items to your store by using a CSV file from you spreadshirt account.
5. Customize
    - Backup the existing templates
    - Customize your template.
    - You can use the Twitter Bootsrap CSS classes or not.

---

##Changelog

- Initial release version 0.90

---

##Roadmap
- Forgotten password email reset.
- Caching to improve performance.
- Integration with Google analytics.
- Infinite scroll for stores with a large number of items.
- A shopping cart with full integration of the spreadshirt API.
- Various fixes.