
# Communication Protocols

Truyền nhận dữ liệu trong vi điều khiển (MCU) là quá trình trao đổi tín hiệu điện áp giữa các chân (pin) của MCU. Trao đổi các tín hiệu 0 1 tạo thành các dữ liệu cần truyền.

Các giao thức định nghĩa cách thức các thiết bị trao đổi dữ liệu, bao gồm định dạng dữ liệu, tốc độ truyền, kiểm soát lỗi và các quy tắc đồng bộ hóa.


## SPI
SPI (Serial Peripheral Interface) là một giao thức truyền thông nối tiếp đồng bộ, thường được sử dụng để kết nối các thiết bị ngoại vi với vi điều khiển. Nó cho phép truyền dữ liệu nhanh chóng và hiệu quả.

**Các đặc điểm của SPI**
- Chuẩn giao tiếp nối tiếp
- Đồng bộ
- Hoạt động ở chế độ song công (Có thể truyền - nhận cùng thời điểm).
- Một Master có thể giao tiếp với nhiều Slave

**Cách kết nối các thiết bị SPI**

Master và Slave: Trong một hệ thống SPI, luôn có một thiết bị đóng vai trò là master  và một hoặc nhiều thiết bị đóng vai trò là slave. Master sẽ điều khiển quá trình truyền dữ liệu.

Mỗi thiết bị gồm 4 chân: 

- SCK (Serial Clock): Thiết bị Master tạo xung tín hiệu SCK và cung cấp cho Slave.
- MISO (Master Input Slave Output): Tín hiệu tạo bởi thiết bị Slave và nhận bởi thiết bị Master.
- MOSI (Master Output Slave Input): Tín hiệu tạo bởi thiết bị Master và nhận bởi thiết bị Slave. 
- SS (Đôi khi CS- Slave Select/Chip Select): Chọn thiết bị Slave cụ thể để giao tiếp. Để chọn Slave giao tiếp thiết bị Master chủ động kéo đường SS tương ứng xuống mức 0 (Low). 

Các chân SCK, MOSI và MISO của tất cả các thiết bị được nối chung. Mỗi slave có một chân SS riêng biệt.
![1](https://github.com/user-attachments/assets/f77da223-86ae-43f5-b529-7cbc91c1ba99)

**Cách hoạt động của SPI**

Bắt đầu quá trình, master sẽ kéo chân CS của slave muốn giao tiếp xuống 0 để báo hiệu muốn truyền nhận.

Master gửi dữ liệu trên đường MOSI đồng thời tạo xung clock trên đường SCK. Mỗi bit được gửi đều được kèm theo 1 xung clock. Slave nhận dữ liệu và gửi dữ liệu trả lời (nếu có) trên đường MISO.

Các thanh ghi cập nhật giá trị và dịch 1 bit.

Lặp lại quá trình trên đến khi truyền xong 8 bit trong thanh ghi
(Giao tiếp song công).

Sau khi truyền xong, master kéo cao tín hiệu SS.
![2](https://github.com/user-attachments/assets/897452e4-0b6c-44eb-93b1-732413be010f)
**CPOL (Clock Polarity)**: CPOL xác định trạng thái của tín hiệu SCK khi không có dữ liệu đang được truyền (tức là trạng thái nghỉ).

- CPOL = 0: Tín hiệu SCK ở trạng thái thấp khi không hoạt động.
- CPOL = 1: Tín hiệu SCK ở trạng thái cao khi không hoạt động.

**CPHA (Clock Phase)**: CPHA xác định thời điểm dữ liệu được lấy mẫu (sampling) so với xung clock.

- CPHA = 0: Dữ liệu được lấy mẫu ở cạnh lên của xung clock.
- CPHA = 1: Dữ liệu được lấy mẫu ở cạnh xuống của xung clock.
![3](https://github.com/user-attachments/assets/b4a106c2-caaf-478e-8508-44b7eef07701)

**Ưu điểm của SPI**:
- Tốc độ cao: SPI có thể đạt tốc độ truyền dữ liệu rất cao, đặc biệt phù hợp với các ứng dụng yêu cầu băng thông lớn.
- Đơn giản: Tương đối dễ hiểu và triển khai, không yêu cầu quá nhiều tài nguyên phần cứng.
- Linh hoạt: SPI hỗ trợ truyền dữ liệu toàn phần (full-duplex).

**Nhược điểm của SPI**
- Cần đồng bộ hóa: Tất cả các thiết bị trên bus SPI phải được đồng bộ hóa với xung clock.
- Không có cơ chế kiểm soát lỗi: SPI không có cơ chế kiểm soát lỗi mạnh mẽ như các giao thức khác, do đó dễ xảy ra lỗi truyền dữ liệu.





## I2C
I2C (Inter-Integrated Circuit) là một giao thức truyền thông nối tiếp đơn giản, được sử dụng để kết nối nhiều thiết bị với nhau trong một hệ thống nhúng. Nó chỉ yêu cầu hai dây tín hiệu: SCL (Serial Clock) và SDA (Serial Data).

**Các đặc điểm của I2C**
- Chuẩn giao tiếp nối tiếp
- Đồng bộ
- Hoạt động ở chế độ bán song công
- Một Master có thể giao tiếp với nhiều Slave

**Cách kết nối các thiết bị I2C**

Mỗi thiết bị gồm 2 chân:
- SCL (Serial Clock): Tạo xung tín hiệu để đồng bộ việc truyền/nhận dữ liệu với các Slave.
- SDA (Serial Data): Chân chứa dữ liệu được truyền đi.
Các chân SCL và SDA của tất cả các thiết bị được nối chung. Mỗi thiết bị I2C có một địa chỉ duy nhất để master có thể xác định và giao tiếp.
![4](https://github.com/user-attachments/assets/6a0ef315-1abf-4c0e-9db4-4bcbe44b32da)
**Cách hoạt động của I2C**

Master bắt đầu giao tiếp bằng cách tạo một điều kiện bắt đầu (Start condition): Đặt chân SDA xuống 0 trước và đặt chân SCL xuống 0.

Master gửi địa chỉ 7 hoặc 10 bit của slave mà nó muốn giao tiếp. Kèm theo đó là 1 bit để đặt lệnh ghi hoặc đọc dữ liệu. Thường là 1 byte. 

Slave nhận được địa chỉ sẽ gửi tín hiệu xác nhận. Master gửi dữ liệu và slave nhận dữ liệu. 

Khi giao tiếp hoàn tất, master tạo một điều kiện kết thúc (Stop condition): Đặt chân SCL lên 1 trước và đặt chân SDA lên 1.

**Ưu điểm của I2C**
- Đơn giản: Chỉ cần hai dây tín hiệu, dễ dàng triển khai.
- Linh hoạt: Có thể kết nối nhiều thiết bị slave với một master.
- Tiêu thụ điện năng thấp: Khi không có dữ liệu truyền, các thiết bị có thể chuyển sang chế độ nghỉ.

**Nhược điểm của I2C**
- Tốc độ: Tốc độ truyền dữ liệu thường chậm hơn so với các giao thức khác như SPI.
- Độ dài bus: Khoảng cách giữa các thiết bị bị giới hạn, thường chỉ vài mét.
- Xung đột bus: Nếu nhiều thiết bị cố gắng truy cập bus cùng lúc, có thể xảy ra xung đột.
## UART
UART (Universal Asynchronous Receiver-Transmitter) là một giao thức truyền thông nối tiếp không đồng bộ, được sử dụng rộng rãi để kết nối các thiết bị điện tử với nhau. Nó đơn giản, dễ triển khai và thường được sử dụng trong các ứng dụng không yêu cầu tốc độ truyền dữ liệu quá cao.

**Các đặc điểm của UART**
- Chuẩn giao tiếp nối tiếp
- Không đồng bộ
- Chỉ 2 thiết bị giao tiếp với nhau
- Hoạt động ở chế độ song công
**Cách kết nối các thiết bị UART**

Mỗi thiết bị gồm 2 chân:
- Tx (Transmit): Chân truyền dữ liệu.
- Rx (Receive): Chân nhận dữ liệu.

Chân TX của thiết bị này được nối với chân RX của thiết bị kia và ngược lại. Các thiết bị cần sử dụng cùng một điện áp cấp nguồn. Các mức logic của tín hiệu TX và RX phải tương thích.
![1](https://github.com/user-attachments/assets/c18b9d61-764d-4270-9bfe-218f61cbfbcc)
**Cách hoạt động của UART**

Xác định baudrate (số bits truyền được/1s) được sử dụng, đồng bộ giữa 2 thiết bị. 

Dữ liệu được truyền dưới dạng các khung dữ liệu, mỗi khung bao gồm các phần: bit bắt đầu, bit dữ liệu, bit chẵn lẻ (tùy chọn) và bit dừng.
![2](https://github.com/user-attachments/assets/05e7218e-a491-47f3-b847-17d98bf742b8)
Khung dữ liệu được xác định phụ thuộc vào nhu cầu truyền nhận. 

Thiết bị truyền bắt đầu bằng việc gửi một bit bắt đầu (thường là 0), sau đó gửi các bit dữ liệu, bit chẵn lẻ (nếu có) và cuối cùng là một hoặc nhiều bit dừng (thường là 1).

Thiết bị nhận phát hiện bit bắt đầu và đồng bộ hóa với xung đồng hồ để nhận các bit dữ liệu. Không sử dụng xung clock như SPI và I2C nên UART phải tạo khoảng delay bằng nhau và phải tự đồng bộ giữa 2 thiết bị.

**Parity bit**

Parity bit là một bit bổ sung được thêm vào một chuỗi dữ liệu nhị phân để kiểm tra lỗi trong quá trình truyền dữ liệu. Nó hoạt động như một cơ chế đơn giản để phát hiện lỗi, giúp đảm bảo tính toàn vẹn của dữ liệu khi truyền qua các kênh truyền thông, đặc biệt là trong giao tiếp UART.

Có 2 loại Parity bits chính:
- Parity chẵn (Even Parity): Số lượng bit 1 trong khung dữ liệu (bao gồm cả parity bit) luôn là số chẵn.
- Parity lẻ (Odd Parity): Số lượng bit 1 trong khung dữ liệu (bao gồm cả parity bit) luôn là số lẻ.
Cách hoạt động: 
- Thêm parity bit vào khung dữ liệu dựa trên loại parity đã chọn (chẵn hoặc lẻ).
- Nhận khung dữ liệu và tính tổng số bit 1.
- So sánh tổng số bit 1 với loại parity đã định. Nếu không khớp, có lỗi xảy ra trong quá trình truyền.
*Parity bit có ưu điểm khá dễ đơn giản sử dụng, phát hiện được những lỗi phổ biến . Nhưng không thể phát hiện toàn bộ lỗi, không thể sửa lỗi và phải tốn thêm 1bit dữ liệu mỗi khung dữ liệu được truyền.*

**Ưu điểm của UART**
- Đơn giản: Chỉ yêu cầu hai dây tín hiệu (TX và RX), dễ dàng triển khai phần cứng và phần mềm.
- Linh hoạt: Có thể cấu hình tốc độ truyền dữ liệu, số bit dữ liệu, bit chẵn lẻ và số bit dừng.
- Phổ biến: Được hỗ trợ rộng rãi bởi nhiều vi điều khiển và các thiết bị ngoại vi.
**Nhược điểm của UART**
- Tốc độ: Tốc độ truyền dữ liệu thường chậm hơn so với các giao thức khác như SPI.
- Nhiễu: Dễ bị ảnh hưởng bởi nhiễu điện từ, đặc biệt khi truyền dữ liệu qua khoảng cách xa.
- Không đồng bộ: Cần có cơ chế đồng bộ hóa giữa thiết bị truyền và nhận.