public class ReservationDTO
{
    public int Id { get; set; }
    public int ScreeningId { get; set; }
    public int SeatId { get; set; }
    public DateTime ReservationTime { get; set; }
    public decimal Price { get; set; }
    public string Status { get; set; }
}

public class CreateReservationDTO
{
    public int ScreeningId { get; set; }
    public int SeatId { get; set; }
} 