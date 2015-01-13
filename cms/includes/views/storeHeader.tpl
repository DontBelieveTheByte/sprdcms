
<!DOCTYPE html>
<html lang="en"><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    <head>
        <title><?php echo $renderable['store'] . " | " . $renderable['name']; ?></title>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <meta name="description" content="<?php echo $renderable['name'] . " " . $renderable['store']; ?>">
        <meta name="author" content="<?php echo "Created by : s" . $renderable['store']; ?>">
        <link rel="shortcut icon" href="theme/favicon.png" />
        <style type="text/css">
            body {
                padding-top: 10px;
                padding-bottom: 40px;
            }
            .articles {
                margin: 5px;
            }

            .article_iframe {
                frameBorder:0;
                margin-top: 20px;
                margin-bottom: 30px;
                /*width: 700px; height and width must match your Spreadshirt template*/
                height: 1800px;
            }
            .category {
                margin-left: auto;
                margin-right: auto;
            }
            /* Custom container */
            .container-narrow {
                margin: 0 auto;
                max-width: 1000px;
            }
            .container-narrow > hr {
                margin: 30px 0;
            }

            /* Main marketing message and sign up button */
            .jumbotron {
                margin: 30px 0px 50px 0px;
                text-align: center;
            }
            .jumbotron h1 {
                font-size: 72px;
                line-height: 1;
            }

        </style>
        <base href="<?php echo $renderable['basePath'] ?>" />
        <link rel="icon" href="misc/favicon.png" />
        <link href='theme/bootstrap/css/bootstrap.min.css' rel="stylesheet">
        <script src="misc/js/jquery-1.9.1.min.js"></script>
        <script src="theme/bootstrap/js/bootstrap.min.js"></script>
        <!--<script src="misc/js/storeFront.js"></script> will implement later-->
    </head>
    <body>
        <div class="container-narrow">
            <div class="row">
                <span class="pull-right">
                    <h3>
                        <a target='_blank' href='https://<?php echo $renderable['shopId']; ?>.Spreadshirt.<?php echo $renderable['shopDomain'] ?>/shop/basket'>Checkout
                            <i class="icon-shopping-cart"></i>
                        </a>
                    </h3>
                </span>
            </div>
            <div class="jumbotron">
                <a style="text-decoration:none; color:black;" href="<?php echo $renderable['basePath'] ?>"><h1>My Super Awesome T-Shirts!</h1></a>
            </div>

            <div class="navbar">
                <div class='navbar-inner'>

                    <ul class="nav">
                        <?php
                        foreach ($renderable['catList'] as $machineName => $category) {
                            echo ($renderable['name'] === $category) ? "<li class='active'>" : "<li>";
                            echo "<a  href='" . $renderable['basePath'];
                            echo ($renderable['modRewrite']) ? "category/$machineName" : "index.php?q=category/$machineName";
                            echo "'><h4>" . $category . "</h4></a></li>";
                        }
                        ?>
                    </ul>
                </div>
            </div>
            <hr>