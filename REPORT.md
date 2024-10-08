## 1: In the Embedded part of the exercise, how can Bluetooth be replaced by RFID? Describe your solution.
> Replace Bluetooth devices with RFID tags. Each user or device would have a unique RFID tag associated with it. RFID tags can store a unique identifier called the BD_ADDR (Bluetooth Device Address), that can be read wirelessly by an RFID reader at the authentication points where Bluetooth devices were previously used. These readers will communicate wirelessly with the RFID tags.
>
> We should maintain a database that maps RFID tag identifiers to authorized users or devices. When an RFID tag is scanned by a reader, the reader sends the tag's identifier to a central server for authentication. Upon scanning an RFID tag, the reader sends the tag's identifier to the authentication server. The server looks up the identifier in the database to verify if it corresponds to an authorized user or device. Additionally, granting (if the RFID tag is associated with an authorized user or device) or denying (otherwise) access based on the authentication result is required.
>
> Security: Implement security measures to prevent unauthorized access to RFID tags or cloning of tags. This may include encryption of RFID tag data, using secure communication protocols between readers and the authentication server, and physical security measures to protect RFID readers from tampering.
---
## Is it possible to transfer the necessary information (person ID) between Bluetooth modules without having to pair them? Explain.
> Yes, it is possible to transfer the necessary information (such as a person's ID) between Bluetooth modules without having to pair them.
>
> We can use Bluetooth Low Energy (BLE) Beacons which are a common solution for transmitting information between Bluetooth devices without the need for pairing. Beacons broadcast small packets of data at regular intervals, which can be received by nearby Bluetooth-enabled devices. they  operate in advertising mode, where they continuously broadcast packets containing information such as a unique identifier (UUID), major and minor values, and optionally additional data payload. These packets can be received by any Bluetooth-enabled device within range.
>
> Each BLE beacon is configured with a unique identifier (UUID) that distinguishes it from other beacons. This UUID can be used to identify specific devices or locations. In addition to the UUID, BLE beacons can include additional data in their advertising packets, such as a person's ID or other relevant information. This data payload can be parsed by nearby devices to extract the necessary information.
>
> Security: While BLE beacons provide a convenient way to transmit information without pairing, it's important to note that security is limited. Since the data is broadcast openly, anyone within range can potentially intercept or spoof the communication. Therefore, BLE beacon-based systems may be more suitable for scenarios where security requirements are relatively low, such as proximity-based applications or indoor navigation systems.
>
> Application Layer Security: To enhance security, additional encryption or authentication mechanisms can be implemented at the application layer. For example, the transmitted data payload can be encrypted using a shared secret or authenticated using digital signatures to prevent tampering or unauthorized access.
---
## 3: Assuming the use of Bluetooth, what solution do you suggest to replace the use of a timer to close the entrance of the company? Explain.
> Bluetooth Presence Detection: Instead of relying solely on a timer, leverage Bluetooth presence detection to monitor the presence of authorized devices near the entrance. This can be achieved using Bluetooth Low Energy (BLE) technology, which allows for low-power, continuous scanning for nearby Bluetooth devices.
>
> Dynamic Door Control: Implement dynamic door control logic based on the presence or absence of authorized Bluetooth devices. When an authorized device is detected nearby, keep the door open to allow entry. Conversely, if no authorized devices are detected within a certain timeframe or if all authorized devices move out of range, initiate the door closing process.
>
> Continuous Monitoring: Have a Bluetooth module or device continuously scan for the presence of authorized Bluetooth devices in the vicinity of the entrance. This scanning process can be performed in the background without requiring active participation from the user.
>
> Fallback Mechanism: In case of Bluetooth connectivity issues or device malfunctions, implement a fallback mechanism to ensure reliable door operation. This could involve reverting to a timer-based closing mechanism as a backup solution or triggering an alert/notification for manual intervention.