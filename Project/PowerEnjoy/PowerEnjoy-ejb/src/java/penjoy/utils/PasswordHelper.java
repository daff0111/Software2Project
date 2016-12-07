/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package penjoy.utils;

import java.math.BigInteger;
import java.security.SecureRandom;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 *
 * A utility class for creating and hashing passwords
 *
 * @author Domenico
 */
public class PasswordHelper {

    private static SecureRandom m_randomGenerator = new SecureRandom();

    //Generate a Random Password
    public static String generatePassword() {
        return new BigInteger(75, m_randomGenerator).toString(32);
    }

    //Method to hash password with MD5
    public static String hashPassword(String password) {
        //No password sent
        if (password == null) {
            return null;
        }
        try {
            MessageDigest digest = MessageDigest.getInstance("MD5");

            digest.update(password.getBytes(), 0, password.length());

            String hashedPassword = new BigInteger(1, digest.digest()).toString(16);
            
            return hashedPassword;
        } catch (NoSuchAlgorithmException e) {

            e.printStackTrace();
        }
        
        return null;
    }

}
