package by.model;

import lombok.Data;
import org.springframework.data.annotation.CreatedDate;
import org.springframework.data.annotation.LastModifiedDate;
import javax.persistence.*;
import java.util.Date;

@MappedSuperclass
@Data
public class BaseEntity {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id")
    private Long id;

//    @CreatedDate
//    @Column(name = "created")
//    @Temporal(TemporalType.TIMESTAMP)
//    private Date created;
//
//    @LastModifiedDate
//    @Column(name = "updated")
//    @Temporal(TemporalType.TIMESTAMP)
//    private Date updated;
}