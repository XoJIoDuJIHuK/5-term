package by.services;

import by.model.Order;
import by.model.Offer;
import by.model.User;
import by.repository.OrderRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.jpa.repository.Modifying;
import org.springframework.stereotype.Service;
import javax.transaction.Transactional;
import java.util.List;
import java.util.Objects;

@Service
public class OrderService {
    final
    OrderRepository orderRepository;

    public OrderService(OrderRepository orderRepository) {
        this.orderRepository = orderRepository;
    }

    public List<Order> getAll() {
        return orderRepository.findAll();
    }

    public List<Order> getByUser(User user) {
        return orderRepository.getOrdersByUserId(user.getId());
    }

    public void save(Order order) {
        orderRepository.save(order);
    }

    @Transactional
    public void deleteByOffer(Offer offer) {
        orderRepository.deleteByOfferId(offer.getId());
    }

    @Transactional
    public int deleteByOfferAndUser(Offer offer, User user) {
        return orderRepository.deleteByOfferAndUser(offer.getId(), user.getId());
    }
    public Order findByOfferAndUser(Offer offer, User user) {
        return orderRepository.findByOfferAndUser(offer, user);
    }

    public List<Order> findAllByOffer(Offer offer) {
        return orderRepository.findAllByOffer(offer);
    }

    public boolean alreadyOrdered(Offer offer, User user) {
        return orderRepository.findAll().stream().anyMatch(e ->
                Objects.equals(e.getOffer().getId(), offer.getId()) &&
                Objects.equals(e.getUser().getId(),  user.getId()));
    }
}