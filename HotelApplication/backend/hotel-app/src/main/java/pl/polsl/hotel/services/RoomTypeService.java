package pl.polsl.hotel.services;

import org.springframework.beans.factory.annotation.Autowired;

import org.springframework.stereotype.Component;
import pl.polsl.hotel.models.*;
import pl.polsl.hotel.repositories.ParametersRepository;
import pl.polsl.hotel.repositories.ReservationRepository;
import pl.polsl.hotel.repositories.RoomTypeRepository;


import java.time.LocalDate;
import java.util.*;

@Component
public class RoomTypeService extends MySession implements StartUpFiller {
    private final RoomTypeRepository roomTypeRepository;
    private final ReservationRepository reservationRepository;
    private final ParametersRepository parametersRepository;
    private Random generator;
    private ArrayList<RoomType> roomsAvailable;
    private LocalDate startLocalDate;
    private Date startDate;
    private LocalDate endLocalDate;
    private Date endDate;
    @Autowired
    public RoomTypeService(RoomTypeRepository roomTypeRepository, ReservationRepository reservationRepository, ParametersRepository parametersRepository) {
        this.roomTypeRepository = roomTypeRepository;
        this.parametersRepository = parametersRepository;
        this.generator = new Random();
        this.roomsAvailable = new ArrayList<RoomType>();
        this.reservationRepository = reservationRepository;
    }

    public Optional<RoomType> findById(Long id) {
        return roomTypeRepository.findById(id);
    }

    public List<RoomType> findAll() {
        return roomTypeRepository.findAll();

    }

    public List<RoomType> getRoomsAvailable(String start, String end) {
        roomsAvailable.clear();
        int index = 0;
        boolean isAvailable = true;
        startLocalDate = LocalDate.parse(start);
        startDate = convertToDate(startLocalDate);
        Calendar startCal = Calendar.getInstance();
        startCal.setTime(startDate);
        startCal.add(Calendar.HOUR, 23);
        startDate = startCal.getTime();
        endLocalDate = LocalDate.parse(end);
        endDate = convertToDate(endLocalDate);

        for (RoomType room : roomTypeRepository.findAll()) {
            isAvailable = true;
           /* index = 0;
            for (LocalDate date: room.getAvailableDates()) {

                if((date.getYear() == fromLocalDate.getYear())&&(date.getMonth() == fromLocalDate.getMonth())&& (date.getDayOfMonth() == fromLocalDate.getDayOfMonth()))
                {
                    if(room.getIsAvailable().get(index))
                    {
                        for(int i = index + 1; i < index + numberOfDays;i++)
                        {
                            if(!room.getIsAvailable().get(i))
                            {
                                isAvailable = false;
                                break;
                            }
                        }
                        if(isAvailable)
                        {*/

                            for (Reservation reservation : reservationRepository.findAll())
                            {
                                if(reservation.getRoom().getId() == room.getId() && ((startDate.after(reservation.getStartDate()) && startDate.before(reservation.getEndDate())) || (endDate.before(reservation.getEndDate()) && endDate.after(reservation.getStartDate()))))
                                {
                                    isAvailable = false;
                                    break;
                                }
                            }
                            if(isAvailable)
                                roomsAvailable.add(room);/*
                        }
                    }
                    break;
                }
                index++;
            }*/

        }
        return roomsAvailable;
    }



    public RoomTypeView save(RoomTypeView room) {

        return map(roomTypeRepository.save(map(room)));
    }

    public void deleteById(Long id) {
        roomTypeRepository.deleteById(id);
    }

    public void createInitialData() throws RuntimeException {

        //if(this.session.get(Admin.class.getSimpleName(),1) == null)
       /* if("tak" == null)
        {
            generator = new Random();
            Room room1 = new Room();
            room1.setNumberOfBeds(generator.nextInt(3) + 1);
            room1.setStorey(generator.nextInt(9));
            room1.setBalcony(generator.nextBoolean());
            room1.setBeautifulViewFromTheWindows(generator.nextBoolean());
            room1.setCloseToElevator(generator.nextBoolean());
            room1.setTypeOfPillow(PillowType.NATURAL) ;
            room1.setCanBeReserved(generator.nextBoolean());
            room1.setPriceForOneDay(10 * generator.nextInt(5) + 50);
            room1.setReadyToUseOnAGivenDay(generator.nextBoolean());


            Room room2 = new Room();
            room2.setNumberOfBeds(generator.nextInt(3) + 1);
            room2.setStorey(generator.nextInt(9));
            room2.setBalcony(generator.nextBoolean());
            room2.setBeautifulViewFromTheWindows(generator.nextBoolean());
            room2.setCloseToElevator(generator.nextBoolean());
            room2.setTypeOfPillow(PillowType.ANTIALLERGIC) ;
            room2.setCanBeReserved(generator.nextBoolean());
            room2.setPriceForOneDay(10 * generator.nextInt(50) + 50);
            room2.setReadyToUseOnAGivenDay(generator.nextBoolean());


            roomRepository.saveAll(Arrays.asList(room1,room2));
        }*/
    }

    public Date convertToDate (LocalDate dateToConvert) {
        return java.sql.Date.valueOf(dateToConvert);
    }
    public RoomType map(RoomTypeView roomTypeView) {
        RoomType roomType = new RoomType();
        roomType.setId(roomTypeView.getId());
        roomType.setHotelId(roomTypeView.getHotelId());
        roomType.setNumberOfRoomsAvailable(roomTypeView.getNumberOfRoomsAvailable());

        if(roomTypeView.getRoomTypeParametersId() != null)
            roomType.setRoomTypeParameters(parametersRepository.getById(roomTypeView.getRoomTypeParametersId()));

        return roomType;
    }

    public RoomTypeView map(RoomType roomType) {
        RoomTypeView roomTypeView = new RoomTypeView();

        roomTypeView.setId(roomType.getId());
        roomTypeView.setHotelId(roomType.getHotelId());
        roomTypeView.setNumberOfRoomsAvailable(roomType.getNumberOfRoomsAvailable());

        if(roomType.getRoomTypeParameters()!= null)
            roomTypeView.setRoomTypeParametersId(roomType.getRoomTypeParameters().getId());
        return roomTypeView;
    }

}