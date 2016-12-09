/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package penjoy.utils;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 *
 * @author Domenico Class to Validate Strings using Regexp
 */
public class LocationHelper {

    public class Location {

        private float lat;
        private float lng;

        Location(float lat, float lng) {
            this.lat = lat;
            this.lng = lng;
        }

        Location(String location) {
            this.lat = LocationHelper.getLatitude(location);
            this.lng = LocationHelper.getLongitude(location);
        }

        public float getLatitude() {
            return this.lat;
        }

        public float getLongitude() {
            return this.lng;
        }
    };

    //Validates PaymentInfo with regexp PAYMENT_PATTERN: 12-32 Numbers
    public static float getLatitude(String location) {
        return Float.parseFloat(location.substring(0, location.indexOf(",")));
    }

    public static float getLongitude(String location) {
        return Float.parseFloat(location.substring(location.indexOf(",") + 1));
    }
}
