#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <String.h>
#include <WiFi.h>
#include <WebServer.h>
WebServer server(80);

const char* ssid = "advprojectshub";      // WiFi SSID
const char* password = "advprojectshub";  // WiFi Password

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C LCD at 0x27

// Define IR sensor pins for 8 slots
#define SLOT1 13
#define SLOT2 12
#define SLOT3 14
#define SLOT4 27
#define SLOT5 26
#define SLOT6 25
#define SLOT7 18
#define SLOT8 19

void setup() {
  Serial.begin(115200);

  // Initialize all 8 sensor pins as inputs
  pinMode(SLOT1, INPUT);
  pinMode(SLOT2, INPUT);
  pinMode(SLOT3, INPUT);
  pinMode(SLOT4, INPUT);
  pinMode(SLOT5, INPUT);
  pinMode(SLOT6, INPUT);
  pinMode(SLOT7, INPUT);
  pinMode(SLOT8, INPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("welcome to the");
  lcd.setCursor(0, 1);
  lcd.print("    project    ");
  delay(3000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("iot based park");
  lcd.setCursor(0, 1);
  lcd.print("    system    ");
  delay(3000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("ssid:");
  lcd.setCursor(0, 1);
  lcd.print(ssid);
  delay(3000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("pswd:");
  lcd.setCursor(0, 1);
  lcd.print(password);
  delay(3000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Connecting...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(3000);
  lcd.clear();

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
  updateLCD();
}

void updateLCD() {
  int slot1_status = digitalRead(SLOT1);
  int slot2_status = digitalRead(SLOT2);
  int slot3_status = digitalRead(SLOT3);
  int slot4_status = digitalRead(SLOT4);
  int slot5_status = digitalRead(SLOT5);
  int slot6_status = digitalRead(SLOT6);
  int slot7_status = digitalRead(SLOT7);
  int slot8_status = digitalRead(SLOT8);

  lcd.clear();
  
  // First row - Slots 1-4
  lcd.setCursor(0, 0);
  lcd.print("1:");
  lcd.print(slot1_status ? "E " : "F ");
  
  lcd.setCursor(4, 0);
  lcd.print("2:");
  lcd.print(slot2_status ? "E " : "F ");
  
  lcd.setCursor(8, 0);
  lcd.print("3:");
  lcd.print(slot3_status ? "E " : "F ");
  
  lcd.setCursor(12, 0);
  lcd.print("4:");
  lcd.print(slot4_status ? "E" : "F");
  
  // Second row - Slots 5-8
  lcd.setCursor(0, 1);
  lcd.print("5:");
  lcd.print(slot5_status ? "E " : "F ");
  
  lcd.setCursor(4, 1);
  lcd.print("6:");
  lcd.print(slot6_status ? "E " : "F ");
  
  lcd.setCursor(8, 1);
  lcd.print("7:");
  lcd.print(slot7_status ? "E " : "F ");
  
  lcd.setCursor(12, 1);
  lcd.print("8:");
  lcd.print(slot8_status ? "E" : "F");
  
  delay(200);
}

String generateSlotHTML(int slotStatus, String slotNumber, String zone) {
  String slotHTML = "<div class='slot'>";
  slotHTML += "<p><strong>Slot " + slotNumber + " (Zone " + zone + ")</strong></p>";
  if (!slotStatus) {
    slotHTML += "<img src='https://cdn-icons-png.flaticon.com/128/741/741407.png' class='car'>";
    slotHTML += "<p>🚗 <strong>Occupied</strong></p>";
  } else {
    slotHTML += "<p>🔲 <strong>Empty</strong></p>";
  }
  slotHTML += "</div>";
  return slotHTML;
}

void handleRoot() {
  int slot1_status = digitalRead(SLOT1);
  int slot2_status = digitalRead(SLOT2);
  int slot3_status = digitalRead(SLOT3);
  int slot4_status = digitalRead(SLOT4);
  int slot5_status = digitalRead(SLOT5);
  int slot6_status = digitalRead(SLOT6);
  int slot7_status = digitalRead(SLOT7);
  int slot8_status = digitalRead(SLOT8);
  
  int totalSlots = 8;
  int occupiedSlots = (!slot1_status) + (!slot2_status) + (!slot3_status) + (!slot4_status) + 
                     (!slot5_status) + (!slot6_status) + (!slot7_status) + (!slot8_status);
  int availableSlots = totalSlots - occupiedSlots;

  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Smart Parking Dashboard - 8 Slots</title>
    <meta http-equiv="refresh" content="2">
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
        :root {
            --primary: #4361ee;
            --success: #2ecc71;
            --danger: #e74c3c;
            --warning: #f39c12;
            --dark: #2c3e50;
            --light: #ecf0f1;
            --gray: #95a5a6;
        }
        
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: 'Segoe UI', system-ui, -apple-system, sans-serif;
        }
        
        body {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
        }
        
        .dashboard {
            max-width: 1400px;
            margin: 0 auto;
        }
        
        .header {
            text-align: center;
            margin-bottom: 30px;
            color: white;
        }
        
        .header h1 {
            font-size: 2.5rem;
            margin-bottom: 10px;
            font-weight: 700;
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 15px;
        }
        
        .header h1 i {
            font-size: 2.8rem;
        }
        
        .status-badge {
            display: inline-block;
            background: rgba(255,255,255,0.2);
            padding: 8px 20px;
            border-radius: 50px;
            font-size: 0.9rem;
            margin-top: 10px;
            backdrop-filter: blur(10px);
        }
        
        .stats-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
            gap: 20px;
            margin-bottom: 30px;
        }
        
        .stat-card {
            background: white;
            border-radius: 15px;
            padding: 25px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.1);
            transition: transform 0.3s ease, box-shadow 0.3s ease;
        }
        
        .stat-card:hover {
            transform: translateY(-5px);
            box-shadow: 0 15px 35px rgba(0,0,0,0.2);
        }
        
        .stat-card h3 {
            color: var(--gray);
            font-size: 0.9rem;
            text-transform: uppercase;
            letter-spacing: 1px;
            margin-bottom: 10px;
        }
        
        .stat-value {
            font-size: 2.5rem;
            font-weight: 700;
            margin-bottom: 10px;
        }
        
        .stat-trend {
            display: flex;
            align-items: center;
            gap: 5px;
            font-size: 0.9rem;
        }
        
        .trend-up { color: var(--success); }
        .trend-down { color: var(--danger); }
        
        .slots-container {
            background: white;
            border-radius: 20px;
            padding: 30px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.1);
            margin-bottom: 30px;
        }
        
        .slots-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 25px;
            flex-wrap: wrap;
        }
        
        .slots-header h2 {
            color: var(--dark);
            font-size: 1.5rem;
        }
        
        .zone-indicator {
            display: flex;
            gap: 15px;
        }
        
        .zone-badge {
            padding: 5px 15px;
            border-radius: 20px;
            font-size: 0.85rem;
            font-weight: 600;
        }
        
        .zone-a { background: #e3f2fd; color: #1976d2; }
        .zone-b { background: #e8f5e9; color: #388e3c; }
        .zone-c { background: #fff3e0; color: #f57c00; }
        .zone-d { background: #fce4ec; color: #c2185b; }
        
        .slots-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
            gap: 20px;
        }
        
        .slot {
            background: var(--light);
            border-radius: 12px;
            padding: 20px;
            display: flex;
            align-items: center;
            gap: 20px;
            transition: all 0.3s ease;
            position: relative;
            overflow: hidden;
        }
        
        .slot::before {
            content: '';
            position: absolute;
            left: 0;
            top: 0;
            height: 100%;
            width: 6px;
        }
        
        .slot.available::before {
            background: var(--success);
        }
        
        .slot.occupied::before {
            background: var(--danger);
        }
        
        .slot-icon {
            font-size: 3rem;
            width: 60px;
            height: 60px;
            display: flex;
            align-items: center;
            justify-content: center;
            border-radius: 12px;
        }
        
        .slot.available .slot-icon {
            background: rgba(46, 204, 113, 0.1);
            color: var(--success);
        }
        
        .slot.occupied .slot-icon {
            background: rgba(231, 76, 60, 0.1);
            color: var(--danger);
        }
        
        .slot-info {
            flex: 1;
        }
        
        .slot-info h4 {
            font-size: 1.2rem;
            margin-bottom: 5px;
            color: var(--dark);
        }
        
        .slot-status {
            font-weight: 600;
            font-size: 1rem;
            padding: 4px 12px;
            border-radius: 20px;
            display: inline-block;
        }
        
        .slot.available .slot-status {
            background: rgba(46, 204, 113, 0.2);
            color: var(--success);
        }
        
        .slot.occupied .slot-status {
            background: rgba(231, 76, 60, 0.2);
            color: var(--danger);
        }
        
        .last-updated {
            color: var(--gray);
            font-size: 0.9rem;
            margin-top: 5px;
        }
        
        .chart-container {
            background: white;
            border-radius: 20px;
            padding: 30px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.1);
        }
        
        .footer {
            text-align: center;
            color: white;
            margin-top: 30px;
            padding-top: 20px;
            border-top: 1px solid rgba(255,255,255,0.1);
        }
        
        .footer-links {
            display: flex;
            justify-content: center;
            gap: 20px;
            margin-top: 15px;
            flex-wrap: wrap;
        }
        
        .footer-links a {
            color: white;
            text-decoration: none;
            opacity: 0.8;
            transition: opacity 0.3s;
        }
        
        .footer-links a:hover {
            opacity: 1;
        }
        
        .pulse {
            display: inline-block;
            width: 8px;
            height: 8px;
            background: var(--success);
            border-radius: 50%;
            margin-right: 8px;
            animation: pulse 1.5s infinite;
        }
        
        @keyframes pulse {
            0% { transform: scale(0.8); opacity: 0.7; }
            50% { transform: scale(1.2); opacity: 1; }
            100% { transform: scale(0.8); opacity: 0.7; }
        }
        
        @media (max-width: 768px) {
            .stats-grid {
                grid-template-columns: 1fr;
            }
            
            .slots-grid {
                grid-template-columns: 1fr;
            }
            
            .header h1 {
                font-size: 2rem;
            }
            
            .slots-header {
                flex-direction: column;
                gap: 10px;
                align-items: flex-start;
            }
        }
    </style>
</head>
<body>
    <div class="dashboard">
        <div class="header">
            <h1>
                <span>🚗</span>
                Smart Parking Dashboard
            </h1>
            <div class="status-badge">
                <span class="pulse"></span>
                Live Monitoring • 8 Slots • Auto-refresh: 2s
            </div>
        </div>
        
        <div class="stats-grid">
            <div class="stat-card">
                <h3>Total Parking Slots</h3>
                <div class="stat-value" style="color: var(--primary);">)rawliteral";
  html += totalSlots;
  html += R"rawliteral(</div>
                <div class="stat-trend">
                    <i class="fas fa-layer-group"></i>
                    <span>4 zones active</span>
                </div>
            </div>
            
            <div class="stat-card">
                <h3>Available Now</h3>
                <div class="stat-value" style="color: var(--success);">)rawliteral";
  html += availableSlots;
  html += R"rawliteral(</div>
                <div class="stat-trend trend-up">
                    <i class="fas fa-arrow-up"></i>
                    <span>Ready for parking</span>
                </div>
            </div>
            
            <div class="stat-card">
                <h3>Currently Occupied</h3>
                <div class="stat-value" style="color: var(--danger);">)rawliteral";
  html += occupiedSlots;
  html += R"rawliteral(</div>
                <div class="stat-trend trend-down">
                    <i class="fas fa-car"></i>
                    <span>In use</span>
                </div>
            </div>
            
            <div class="stat-card">
                <h3>Occupancy Rate</h3>
                <div class="stat-value" style="color: var(--warning);">)rawliteral";
  html += (occupiedSlots * 100) / totalSlots;
  html += R"rawliteral(%</div>
                <div class="stat-trend">
                    <i class="fas fa-chart-pie"></i>
                    <span>Real-time</span>
                </div>
            </div>
        </div>
        
        <div class="slots-container">
            <div class="slots-header">
                <h2>Parking Slots Status</h2>
                <div class="zone-indicator">
                    <span class="zone-badge zone-a">Zone A: Slots 1-2</span>
                    <span class="zone-badge zone-b">Zone B: Slots 3-4</span>
                    <span class="zone-badge zone-c">Zone C: Slots 5-6</span>
                    <span class="zone-badge zone-d">Zone D: Slots 7-8</span>
                </div>
            </div>
            
            <div class="slots-grid">
                <!-- Slot 1 -->
                <div class="slot )rawliteral";
  html += slot1_status ? "available" : "occupied";
  html += R"rawliteral(">
                    <div class="slot-icon">
                        )rawliteral";
  html += slot1_status ? "🅿️" : "🚗";
  html += R"rawliteral(
                    </div>
                    <div class="slot-info">
                        <h4>Slot #1 • Zone A</h4>
                        <div class="slot-status">
                            )rawliteral";
  html += slot1_status ? "AVAILABLE" : "OCCUPIED";
  html += R"rawliteral(
                        </div>
                        <div class="last-updated">
                            )rawliteral";
  html += slot1_status ? "• Ready for parking" : "• Vehicle detected";
  html += R"rawliteral(
                        </div>
                    </div>
                </div>
                
                <!-- Slot 2 -->
                <div class="slot )rawliteral";
  html += slot2_status ? "available" : "occupied";
  html += R"rawliteral(">
                    <div class="slot-icon">
                        )rawliteral";
  html += slot2_status ? "🅿️" : "🚗";
  html += R"rawliteral(
                    </div>
                    <div class="slot-info">
                        <h4>Slot #2 • Zone A</h4>
                        <div class="slot-status">
                            )rawliteral";
  html += slot2_status ? "AVAILABLE" : "OCCUPIED";
  html += R"rawliteral(
                        </div>
                        <div class="last-updated">
                            )rawliteral";
  html += slot2_status ? "• Ready for parking" : "• Vehicle detected";
  html += R"rawliteral(
                        </div>
                    </div>
                </div>
                
                <!-- Slot 3 -->
                <div class="slot )rawliteral";
  html += slot3_status ? "available" : "occupied";
  html += R"rawliteral(">
                    <div class="slot-icon">
                        )rawliteral";
  html += slot3_status ? "🅿️" : "🚗";
  html += R"rawliteral(
                    </div>
                    <div class="slot-info">
                        <h4>Slot #3 • Zone B</h4>
                        <div class="slot-status">
                            )rawliteral";
  html += slot3_status ? "AVAILABLE" : "OCCUPIED";
  html += R"rawliteral(
                        </div>
                        <div class="last-updated">
                            )rawliteral";
  html += slot3_status ? "• Ready for parking" : "• Vehicle detected";
  html += R"rawliteral(
                        </div>
                    </div>
                </div>
                
                <!-- Slot 4 -->
                <div class="slot )rawliteral";
  html += slot4_status ? "available" : "occupied";
  html += R"rawliteral(">
                    <div class="slot-icon">
                        )rawliteral";
  html += slot4_status ? "🅿️" : "🚗";
  html += R"rawliteral(
                    </div>
                    <div class="slot-info">
                        <h4>Slot #4 • Zone B</h4>
                        <div class="slot-status">
                            )rawliteral";
  html += slot4_status ? "AVAILABLE" : "OCCUPIED";
  html += R"rawliteral(
                        </div>
                        <div class="last-updated">
                            )rawliteral";
  html += slot4_status ? "• Ready for parking" : "• Vehicle detected";
  html += R"rawliteral(
                        </div>
                    </div>
                </div>
                
                <!-- Slot 5 -->
                <div class="slot )rawliteral";
  html += slot5_status ? "available" : "occupied";
  html += R"rawliteral(">
                    <div class="slot-icon">
                        )rawliteral";
  html += slot5_status ? "🅿️" : "🚗";
  html += R"rawliteral(
                    </div>
                    <div class="slot-info">
                        <h4>Slot #5 • Zone C</h4>
                        <div class="slot-status">
                            )rawliteral";
  html += slot5_status ? "AVAILABLE" : "OCCUPIED";
  html += R"rawliteral(
                        </div>
                        <div class="last-updated">
                            )rawliteral";
  html += slot5_status ? "• Ready for parking" : "• Vehicle detected";
  html += R"rawliteral(
                        </div>
                    </div>
                </div>
                
                <!-- Slot 6 -->
                <div class="slot )rawliteral";
  html += slot6_status ? "available" : "occupied";
  html += R"rawliteral(">
                    <div class="slot-icon">
                        )rawliteral";
  html += slot6_status ? "🅿️" : "🚗";
  html += R"rawliteral(
                    </div>
                    <div class="slot-info">
                        <h4>Slot #6 • Zone C</h4>
                        <div class="slot-status">
                            )rawliteral";
  html += slot6_status ? "AVAILABLE" : "OCCUPIED";
  html += R"rawliteral(
                        </div>
                        <div class="last-updated">
                            )rawliteral";
  html += slot6_status ? "• Ready for parking" : "• Vehicle detected";
  html += R"rawliteral(
                        </div>
                    </div>
                </div>
                
                <!-- Slot 7 -->
                <div class="slot )rawliteral";
  html += slot7_status ? "available" : "occupied";
  html += R"rawliteral(">
                    <div class="slot-icon">
                        )rawliteral";
  html += slot7_status ? "🅿️" : "🚗";
  html += R"rawliteral(
                    </div>
                    <div class="slot-info">
                        <h4>Slot #7 • Zone D</h4>
                        <div class="slot-status">
                            )rawliteral";
  html += slot7_status ? "AVAILABLE" : "OCCUPIED";
  html += R"rawliteral(
                        </div>
                        <div class="last-updated">
                            )rawliteral";
  html += slot7_status ? "• Ready for parking" : "• Vehicle detected";
  html += R"rawliteral(
                        </div>
                    </div>
                </div>
                
                <!-- Slot 8 -->
                <div class="slot )rawliteral";
  html += slot8_status ? "available" : "occupied";
  html += R"rawliteral(">
                    <div class="slot-icon">
                        )rawliteral";
  html += slot8_status ? "🅿️" : "🚗";
  html += R"rawliteral(
                    </div>
                    <div class="slot-info">
                        <h4>Slot #8 • Zone D</h4>
                        <div class="slot-status">
                            )rawliteral";
  html += slot8_status ? "AVAILABLE" : "OCCUPIED";
  html += R"rawliteral(
                        </div>
                        <div class="last-updated">
                            )rawliteral";
  html += slot8_status ? "• Ready for parking" : "• Vehicle detected";
  html += R"rawliteral(
                        </div>
                    </div>
                </div>
            </div>
        </div>
        
        <div class="chart-container">
            <h2 style="color: var(--dark); margin-bottom: 20px;">Occupancy Overview (8 Slots)</h2>
            <canvas id="parkingChart"></canvas>
        </div>
        
        <div class="footer">
            <p>Smart Parking System v2.0 • 8-Slot Configuration</p>
            <p style="opacity: 0.8; margin-top: 5px;">Connected to: )rawliteral";
  html += WiFi.localIP().toString();
  html += R"rawliteral(</p>
            <div class="footer-links">
                <a href="#"><i class="fas fa-wifi"></i> Network Status</a>
                <a href="#"><i class="fas fa-history"></i> View History</a>
                <a href="#"><i class="fas fa-qrcode"></i> Generate QR</a>
                <a href="#"><i class="fas fa-cog"></i> Settings</a>
            </div>
        </div>
    </div>
    
    <script>
        // Initialize Chart
        const ctx = document.getElementById('parkingChart').getContext('2d');
        const availableSlots = )rawliteral";
  html += availableSlots;
  html += R"rawliteral(;
        const occupiedSlots = )rawliteral";
  html += occupiedSlots;
  html += R"rawliteral(;
        
        const parkingChart = new Chart(ctx, {
            type: 'doughnut',
            data: {
                labels: ['Available', 'Occupied'],
                datasets: [{
                    data: [availableSlots, occupiedSlots],
                    backgroundColor: ['#2ecc71', '#e74c3c'],
                    borderWidth: 2,
                    borderColor: '#fff',
                    hoverOffset: 15
                }]
            },
            options: {
                responsive: true,
                maintainAspectRatio: true,
                plugins: {
                    legend: {
                        position: 'bottom',
                        labels: {
                            padding: 20,
                            usePointStyle: true,
                            font: {
                                size: 14
                            }
                        }
                    },
                    tooltip: {
                        callbacks: {
                            label: function(context) {
                                let label = context.label || '';
                                if (label) {
                                    label += ': ';
                                }
                                label += context.parsed + ' slot';
                                if (context.parsed !== 1) {
                                    label += 's';
                                }
                                return label;
                            }
                        }
                    }
                },
                animation: {
                    animateScale: true,
                    animateRotate: true
                }
            }
        });
        
        // Add hover effect to slots
        document.querySelectorAll('.slot').forEach(slot => {
            slot.addEventListener('mouseenter', function() {
                this.style.transform = 'translateX(5px)';
            });
            
            slot.addEventListener('mouseleave', function() {
                this.style.transform = 'translateX(0)';
            });
        });
    </script>
</body>
</html>)rawliteral";

  server.send(200, "text/html", html);
}