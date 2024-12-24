public interface IReservationService
{
    Task<ReservationDTO> CreateReservationAsync(int userId, CreateReservationDTO reservationDto);
    Task<IEnumerable<ReservationDTO>> GetUserReservationsAsync(int userId);
    Task<ReservationDTO> CancelReservationAsync(int userId, int reservationId);
} 