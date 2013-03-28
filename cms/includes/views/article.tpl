
<?php require_once ('./includes/views/storeHeader.tpl'); ?>
<div class="row">
    <a href='<?php echo $_SERVER['PHP_SELF'] . "?q=article/" . $renderable['article']->id ?>'>
        <h1> <?php echo $renderable['article']->name ?> </h1>
    </a>

</div>
<div id="single_article row">
    <iframe class='article_iframe span9 pull-left' frameBorder="0" seamless name='article_iframe' src='<?php echo $renderable['article']->url ?>'></iframe>
    <div class="itemDescription span3">
        <?php echo $renderable['article']->desc; ?>
    </div>
    <?php
    ?>
</div>
<?php require_once ('./includes/views/storeFooter.tpl'); ?>