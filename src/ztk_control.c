/*
 * Copyright (C) 2018-2020 Alexandros Theodotou <alex at zrythm dot org>
 *
 * This file is part of ZToolkit
 *
 * ZToolkit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * ZToolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU General Affero Public License
 * along with ZToolkit.  If not, see <https://www.gnu.org/licenses/>.
 *
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 * Copyright (C) 2010 Paul Davis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "ztoolkit/ztk.h"

/**
 * Macro to get real value.
 */
#define GET_REAL_VAL \
  ((*self->getter) (self, self->object))

/**
 * MAcro to get real value from knob value.
 */
#define REAL_VAL_FROM_CONTROL(knob) \
  (self->min + (float) knob * \
   (self->max - self->min))

/**
 * Converts from real value to knob value
 */
#define CONTROL_VAL_FROM_REAL(real) \
  (((float) real - self->min) / \
   (self->max - self->min))

/**
 * Sets real val
 */
#define SET_REAL_VAL(real) \
   ((*self->setter) ( \
      self, self->object, (float) real))

static void
update_cb (
  ZtkWidget * w,
  void *      data)
{
  ZtkControl * self = (ZtkControl *) w;
  ZtkApp * app = w->app;
  if (w->state & ZTK_WIDGET_STATE_PRESSED)
    {
      double delta =
        app->prev_press_y -
        app->offset_press_y;

      SET_REAL_VAL (
        REAL_VAL_FROM_CONTROL (
          CLAMP (
            CONTROL_VAL_FROM_REAL (GET_REAL_VAL) +
              0.007f * (float) delta,
             0.0f, 1.0f)));
    }
}

static void
control_free (
  ZtkWidget * widget,
  void *      data)
{
  ZtkControl * self = (ZtkControl *) widget;

  free (self);
}

/**
 * Creates a new control.
 *
 * @param get_val Getter function.
 * @param set_val Setter function.
 * @param draw_cb Custom draw callback.
 * @param object Object to call get/set with.
 */
ZtkControl *
ztk_control_new (
  ZtkRect * rect,
  ZtkControlGetter get_val,
  ZtkControlSetter set_val,
  ZtkWidgetDrawCallback draw_cb,
  void * object,
  float  min,
  float  max,
  float  zero)
{
  ZtkControl * self = calloc (1, sizeof (ZtkControl));
  ztk_widget_init (
    (ZtkWidget *) self, ZTK_WIDGET_TYPE_CONTROL, rect,
    update_cb, draw_cb,
    control_free);

  self->getter = get_val;
  self->setter = set_val;
  self->object = object;
  self->min = min;
  self->max = max;

  return self;
}
