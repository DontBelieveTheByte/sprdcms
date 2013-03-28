
<?php require_once ('./includes/views/storeHeader.tpl'); ?>
<div class="row category">
    <?php
    if (isset($renderable['articles']) && !empty($renderable['articles'])) {
        foreach ($renderable['articles'] as $article) {
            $html = '<div class="articles span4">';
            $html .= "<a href='" . $renderable['basePath'];
            $html .= ($renderable['modRewrite']) ? "article/" : "index.php?q=article/";
            $html .= $article->id . "'>";
            $html .= "<img align='center' src='images/" . $article->img1 . "' onmouseover=this.src='images/" . $article->img0 . "' onmouseout=this.src='images/" . $article->img1 . "'></a>";
            $html .= "<div class='muted'>$" . $article->price . "</div>";
            $html .= "<h3 style='height:100px';>$article->name</h3>";
            $html .= '</div>';
            echo $html;
        }
    }
    ?>
</div>
<?php
require_once ('./includes/views/storeFooter.tpl');
?>
