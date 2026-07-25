/*
 * App Name: MedLink IoT
 * Project: IoT Based Smart Pillbox & Vital Health Monitoring
 * Tech Stack: React.js, Tailwind CSS, Firebase
 */

import React, { useState, useEffect } from 'react';
import { Heart, Activity, Bell, ShieldAlert } from 'lucide-react';

export default function MedLinkApp() {
  const [user, setUser] = useState(null);
  const [vitals, setVitals] = useState({ bpm: 72, spo2: 98 });

  // Simulate Real-time Data Sync from Hardware
  useEffect(() => {
    const interval = setInterval(() => {
      // In production, this fetches from Firebase
      setVitals(prev => ({
        bpm: 68 + Math.floor(Math.random() * 5),
        spo2: 97 + Math.floor(Math.random() * 2)
      }));
    }, 2000);
    return () => clearInterval(interval);
  }, []);

  // --- Login Screen ---
  if (!user) {
    return (
      <div className="flex h-screen bg-slate-50">
        <div className="w-1/2 bg-blue-600 flex items-center justify-center text-white p-12">
          <div>
            <Heart size={64} className="mb-6" />
            <h1 className="text-5xl font-black mb-4">MedLink IoT</h1>
            <p className="text-xl text-blue-100">
              IoT Based Smart Pillbox & Vital Health Monitoring
            </p>
          </div>
        </div>
        <div className="w-1/2 flex items-center justify-center">
          <button
            onClick={() => setUser({ name: "User" })}
            className="px-8 py-4 bg-white border-2 border-slate-200 rounded-2xl text-xl font-bold shadow-lg"
          >
            Sign in to MedLink IoT
          </button>
        </div>
      </div>
    );
  }

  // --- Main Dashboard ---
  return (
    <div className="min-h-screen bg-slate-50 p-8">
      <header className="flex justify-between items-center mb-8">
        <h1 className="text-3xl font-black text-slate-800">MedLink IoT Dashboard</h1>
        <div className="flex gap-2">
          <span className="bg-emerald-100 text-emerald-700 px-4 py-2 rounded-full font-bold text-sm">
            Device Online
          </span>
        </div>
      </header>

      {/* Next Dose Card */}
      <div className="bg-blue-600 text-white p-8 rounded-[32px] shadow-xl mb-8">
        <p className="text-blue-200 font-bold uppercase tracking-widest mb-2">
          Next Dose In
        </p>
        <h2 className="text-7xl font-black">04h 12m</h2>
        <div className="mt-4">
          <p className="text-xl font-bold">Metformin • 500mg</p>
        </div>
      </div>

      {/* Vitals Section */}
      <div className="grid grid-cols-2 gap-6 mb-8">
        <div className="bg-white p-6 rounded-[24px] shadow-sm">
          <div className="flex items-center gap-3 mb-2">
            <Heart className="text-rose-500" />
            <span className="font-bold text-slate-500">Heart Rate</span>
          </div>
          <p className="text-5xl font-black text-slate-800">{vitals.bpm} BPM</p>
        </div>
        <div className="bg-white p-6 rounded-[24px] shadow-sm">
          <div className="flex items-center gap-3 mb-2">
            <Activity className="text-blue-500" />
            <span className="font-bold text-slate-500">SpO2 Level</span>
          </div>
          <p className="text-5xl font-black text-slate-800">{vitals.spo2}%</p>
        </div>
      </div>

      {/* SOS Trigger */}
      <button
        className="w-full bg-rose-500 text-white p-6 rounded-[24px] font-black text-2xl shadow-lg flex items-center justify-center gap-3"
        onClick={() => alert("SOS Alert Sent to Caregiver!")}
      >
        <ShieldAlert size={32} />
        EMERGENCY SOS
      </button>
    </div>
  );
}
