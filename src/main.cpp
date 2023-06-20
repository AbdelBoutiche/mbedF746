#include <mbed.h>
#include <threadLvgl.h>
#include <PwmOut.h>
#include "demos/lv_demos.h"
#include <cstdio>

ThreadLvgl threadLvgl(30);
PwmOut ventilo (PB_14);
AnalogIn sonde(PA_0);

static lv_obj_t * label;
static lv_obj_t * label_sonde;
static lv_obj_t * label_texte;
static lv_obj_t * texte;
static lv_style_t style_btn;
static lv_style_t style_btn_pressed;
static lv_style_t style_btn_red;
float vitesse;
float valeur_sonde;
float valeur_PH;

static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);

    /*Refresh the text*/
    lv_label_set_text_fmt(label, "%"LV_PRId32, lv_slider_get_value(slider));
    lv_obj_align_to(label, slider, LV_ALIGN_OUT_TOP_MID, 0, -15);    /*Align top of the slider*/
    vitesse=lv_slider_get_value(slider)/100.0;
}
static void slider_write(lv_event_t * e)
{
    
    ventilo.write(vitesse);
}
static void slider_stop(lv_event_t * e)
{
    
    ventilo.write(0);
}
void lv_example_get_started_3(void)
{
    /*Create a slider in the center of the display*/
    lv_obj_t * slider = lv_slider_create(lv_scr_act());
    lv_obj_set_width(slider, 100);                          /*Set the width*/
    lv_obj_set_pos(slider,20,250);                                  /*Align to the center of the parent (screen)*/
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);     /*Assign an event function*/

    /*Create a label above the slider*/
    label = lv_label_create(lv_scr_act());
    texte = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "0");
    lv_label_set_text(texte, "Vitesse de melange");
    lv_obj_align_to(label, slider, LV_ALIGN_OUT_TOP_MID, 0, -15); 
    lv_obj_align_to(texte, label, LV_ALIGN_OUT_TOP_MID, 0, -10);   /*Align top of the slider*/
    
}

static lv_color_t darken(const lv_color_filter_dsc_t * dsc, lv_color_t color, lv_opa_t opa)
{
    LV_UNUSED(dsc);
    return lv_color_darken(color, opa);
}

static void style_init(void)
{
    /*Create a simple button style*/
    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, 10);
    lv_style_set_bg_opa(&style_btn, LV_OPA_COVER);
    lv_style_set_bg_color(&style_btn, lv_palette_lighten(LV_PALETTE_GREY, 3));
    lv_style_set_bg_grad_color(&style_btn, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_bg_grad_dir(&style_btn, LV_GRAD_DIR_VER);

    lv_style_set_border_color(&style_btn, lv_color_black());
    lv_style_set_border_opa(&style_btn, LV_OPA_20);
    lv_style_set_border_width(&style_btn, 2);

    lv_style_set_text_color(&style_btn, lv_color_black());

    /*Create a style for the pressed state.
     *Use a color filter to simply modify all colors in this state*/
    static lv_color_filter_dsc_t color_filter;
    lv_color_filter_dsc_init(&color_filter, darken);
    lv_style_init(&style_btn_pressed);
    lv_style_set_color_filter_dsc(&style_btn_pressed, &color_filter);
    lv_style_set_color_filter_opa(&style_btn_pressed, LV_OPA_20);

    /*Create a red style. Change only some colors.*/
    lv_style_init(&style_btn_red);
    lv_style_set_bg_color(&style_btn_red, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_bg_grad_color(&style_btn_red, lv_palette_lighten(LV_PALETTE_RED, 3));
}

void lv_example_style_1()
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_radius(&style, 5);

    /*Make a gradient*/
    lv_style_set_width(&style, 100);
    lv_style_set_height(&style, LV_SIZE_CONTENT);

    lv_style_set_pad_ver(&style, 20);
    lv_style_set_pad_left(&style, 5);

    lv_style_set_x(&style, lv_pct(20));
    lv_style_set_y(&style, lv_pct(20));

    /*Create an object with the new style*/
    lv_obj_t * obj = lv_obj_create(lv_scr_act());
    lv_obj_add_style(obj, &style, 0);

    label_sonde = lv_label_create(obj);

    
}
/**
 * Create styles from scratch for buttons.
 */
void lv_example_get_started_2(void)
{
    /*Initialize the style*/
    style_init();

    /*Create a button and use the new styles*/
    lv_obj_t * btn = lv_btn_create(lv_scr_act());
    /* Remove the styles coming from the theme
     * Note that size and position are also stored as style properties
     * so lv_obj_remove_style_all will remove the set size and position too */
    lv_obj_remove_style_all(btn);
    lv_obj_set_pos(btn, 325, 200);
    lv_obj_set_size(btn, 150, 50);
    lv_obj_add_style(btn, &style_btn, 0);
    lv_obj_add_style(btn, &style_btn_pressed, LV_STATE_PRESSED);

    /*Add a label to the button*/
    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "Lancer le melange");
    lv_obj_center(label);
    lv_obj_add_event_cb(btn, slider_write, LV_EVENT_CLICKED, NULL); 
    /*Create another button and use the red style too*/
    lv_obj_t * btn2 = lv_btn_create(lv_scr_act());
    lv_obj_remove_style_all(btn2);                      
    lv_obj_set_pos(btn2, 175, 200);
    
    lv_obj_set_size(btn2, 150, 50);
    lv_obj_add_style(btn2, &style_btn, 0);
    lv_obj_add_style(btn2, &style_btn_red, 0);
    lv_obj_add_style(btn2, &style_btn_pressed, LV_STATE_PRESSED);
    lv_obj_set_style_radius(btn2, LV_RADIUS_CIRCLE, 0); //Add a local style too

    label = lv_label_create(btn2);
    lv_label_set_text(label, "STOP");
    lv_obj_center(label);
    lv_obj_add_event_cb(btn2, slider_stop, LV_EVENT_CLICKED, NULL); 
}
void lv_example_get_started_1(void)
{
    /*Change the active screen's background color*/
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0xFEFFB3), LV_PART_MAIN);

    /*Create a white label, set its text and align it to the center*/
    label_texte = lv_label_create(lv_scr_act());
    
    
    lv_obj_set_style_text_color(lv_scr_act(), lv_color_hex(0x248389), LV_PART_MAIN);
    lv_obj_align(label_texte, LV_ALIGN_TOP_LEFT, 0, 0);
}
int main() {

    threadLvgl.lock();

    ventilo.pulsewidth(1);
    ventilo.period(1);
    ventilo.write(0);
    lv_example_style_1();//Création du style pour l'onglet texte
    lv_example_get_started_3();//Création du slider
    style_init();//Créer les styles pour les boutons
    lv_example_get_started_2();//Création des 2 boutons stop et lancer
    lv_example_get_started_1();//Création d'un onglet texte
    
    
    
    
    threadLvgl.unlock();
    
    

    while (1) {
        valeur_sonde=sonde.read();
        valeur_PH=(5*valeur_sonde*3.56) - 1.889;
        lv_label_set_text_fmt(label_sonde,"%.0f  PH",valeur_PH);
        if (valeur_PH<8.0 || valeur_PH>6.0)
        {
            
            lv_label_set_text(label_texte, "Tout va bien pour le moment");
        
        }
        if(valeur_PH>8.0){
            lv_label_set_text(label_texte, "Trop basique ajoutez de l'acide");
            
    
        }
        if (valeur_PH<6.0)
        {
            lv_label_set_text(label_texte, "Trop acide ajoutez du basique");
           
    
    
            
        }
        ThisThread::sleep_for(100ms);
    }
}