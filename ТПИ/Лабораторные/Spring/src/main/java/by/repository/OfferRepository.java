package by.repository;

import by.model.Offer;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface OfferRepository extends JpaRepository<Offer, Long> {
    Offer findOfferById(Long id);
    List<Offer> findBySubject(String subject);
}