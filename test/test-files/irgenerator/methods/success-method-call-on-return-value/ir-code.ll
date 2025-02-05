; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Stamp = type { double, i1 }
%struct.Letter = type { ptr, %struct.Stamp }

@printf.str.0 = private unnamed_addr constant [21 x i8] c"Value: %f, glued: %d\00", align 1
@anon.string.0 = private unnamed_addr constant [17 x i8] c"This is a letter\00", align 1
@printf.str.1 = private unnamed_addr constant [17 x i8] c"Stamp glued: %d\0A\00", align 1

define private void @_ZN5Stamp5printEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %value.addr = getelementptr inbounds %struct.Stamp, ptr %2, i64 0, i32 0
  %3 = load double, ptr %value.addr, align 8
  %4 = load ptr, ptr %this, align 8
  %glued.addr = getelementptr inbounds %struct.Stamp, ptr %4, i64 0, i32 1
  %5 = load i1, ptr %glued.addr, align 1
  %6 = zext i1 %5 to i32
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, double %3, i32 %6)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #0

define private %struct.Stamp @_ZN6Letter8getStampEv(ptr noundef nonnull align 8 dereferenceable(24) %0) {
  %result = alloca %struct.Stamp, align 8
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %stamp.addr = getelementptr inbounds %struct.Letter, ptr %2, i64 0, i32 1
  %3 = load %struct.Stamp, ptr %stamp.addr, align 8
  ret %struct.Stamp %3
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  %letter = alloca %struct.Letter, align 8
  %stamp = alloca %struct.Stamp, align 8
  store i32 0, ptr %result, align 4
  store %struct.Letter { ptr @anon.string.0, %struct.Stamp { double 3.400000e+00, i1 true } }, ptr %letter, align 8
  %stamp.addr = getelementptr inbounds %struct.Letter, ptr %letter, i64 0, i32 1
  %glued.addr = getelementptr inbounds %struct.Stamp, ptr %stamp.addr, i64 0, i32 1
  %1 = load i1, ptr %glued.addr, align 1
  %2 = zext i1 %1 to i32
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %2)
  %4 = call %struct.Stamp @_ZN6Letter8getStampEv(ptr noundef nonnull align 8 dereferenceable(24) %letter)
  store %struct.Stamp %4, ptr %stamp, align 8
  call void @_ZN5Stamp5printEv(ptr noundef nonnull align 8 dereferenceable(16) %stamp)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

attributes #0 = { nofree nounwind }
attributes #1 = { noinline nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
