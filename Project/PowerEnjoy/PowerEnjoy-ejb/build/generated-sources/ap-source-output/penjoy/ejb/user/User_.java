package penjoy.ejb.user;

import javax.annotation.Generated;
import javax.persistence.metamodel.SingularAttribute;
import javax.persistence.metamodel.StaticMetamodel;

@Generated(value="EclipseLink-2.5.2.v20140319-rNA", date="2016-12-09T16:23:53")
@StaticMetamodel(User.class)
public class User_ { 

    public static volatile SingularAttribute<User, String> license;
    public static volatile SingularAttribute<User, String> password;
    public static volatile SingularAttribute<User, Boolean> loggedin;
    public static volatile SingularAttribute<User, Long> id;
    public static volatile SingularAttribute<User, String> paymentinfo;
    public static volatile SingularAttribute<User, String> email;
    public static volatile SingularAttribute<User, String> username;

}