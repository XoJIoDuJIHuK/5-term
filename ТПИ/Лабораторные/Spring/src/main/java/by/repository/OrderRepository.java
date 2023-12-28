package by.repository;

import by.model.Offer;
import by.model.Order;
import by.model.User;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface OrderRepository extends JpaRepository<Order, Long> {

    @Modifying
    @Query("DELETE FROM Order o WHERE o.offer.id = :offerId")
    int deleteByOfferId(@Param("offerId") long offerId);

    @Query("SELECT o FROM Order o WHERE o.user.id = :userId")
    List<Order> getOrdersByUserId(@Param("userId") Long id);

    @Modifying
    @Query("DELETE FROM Order o WHERE o.offer.id = :offerId and o.user.id = :userId")
    int deleteByOfferAndUser(@Param("offerId") long offerId, @Param("userId") long userId);

    Order findByOffer(Offer offer);

    List<Order> findAllByOffer(Offer offer);
    Order findByOfferAndUser(Offer offer, User user);
}