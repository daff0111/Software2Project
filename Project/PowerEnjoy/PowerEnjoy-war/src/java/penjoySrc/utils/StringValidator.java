/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package penjoySrc.utils;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 *
 * @author Domenico Class to Validate Strings using Regexp
 */
public class StringValidator {

    private Matcher matcher;
    private final Pattern m_userNamePattern;
    private final Pattern m_emailPattern;
    private final Pattern m_licensePattern;
    private final Pattern m_paymentInfoPattern;

    private static final String USERNAME_PATTERN = "^[A-Za-z0-9_-]{4,32}$";
    private static final String EMAIL_PATTERN = "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$";
    private static final String LICENSE_PATTERN = "^[A-Z]{2}[0-9]{7}[A-Z]{1}$";
    private static final String PAYMENT_PATTERN = "^[0-9]{12,32}$";

    public StringValidator() {
        m_userNamePattern = Pattern.compile(USERNAME_PATTERN);
        m_emailPattern = Pattern.compile(EMAIL_PATTERN);
        m_licensePattern = Pattern.compile(LICENSE_PATTERN);
        m_paymentInfoPattern = Pattern.compile(PAYMENT_PATTERN);
    }

    //Validates Username with regexp USERNAME_PATTERN;
    public boolean validateUsername(String username) {
        if (username.length() > 3 && username.length() < 33) {
            matcher = m_userNamePattern.matcher(username);
            return matcher.matches();
        }
        return false;
    }

    //Validates Email with regexp EMAIL_PATTERN: name@example.com
    public boolean validateEmail(String email) {
        if (email.length() > 3 && email.length() < 33) {
            matcher = m_emailPattern.matcher(email);
            return matcher.matches();
        }
        return false;
    }

    //Validates License with regexp LICENSE_PATTERN: AA0000001B
    public boolean validateLicense(String license) {
        if (license.length() >= 10 && license.length() < 33) {
            matcher = m_licensePattern.matcher(license);
            return matcher.matches();
        }
        return false;
    }

    //Validates PaymentInfo with regexp PAYMENT_PATTERN: 12-32 Numbers
    public boolean validatePayment(String payment) {
        if (payment.length() > 11 && payment.length() < 33) {
            matcher = m_paymentInfoPattern.matcher(payment);
            return matcher.matches();
        }
        return false;
    }
}
