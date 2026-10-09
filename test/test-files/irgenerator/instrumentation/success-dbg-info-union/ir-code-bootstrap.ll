; ModuleID = 'source.spice'
source_filename = "source.spice"

%union.Value = type { i32, [0 x double], [8 x i8] }
%struct.Vec2 = type { i32, i32 }
%union.Box = type { i32, [0 x i64], [8 x i8] }
%union.Box.0 = type { i32, [0 x double], [8 x i8] }
%struct.Holder = type { %union.Value, %union.Box.1 }
%union.Box.1 = type { i32, [0 x i64], [8 x i8] }

@stderr = external local_unnamed_addr global ptr, align 8
@anon.string.0 = private unnamed_addr constant [93 x i8] c"Program panicked at ./source.spice:28:12: active field mismatch on union field access 'vec'\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [93 x i8] c"Program panicked at ./source.spice:28:26: active field mismatch on union field access 'vec'\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [9 x i8] c"Int: %d\0A\00", align 4, !dbg !0
@anon.string.2 = private unnamed_addr constant [91 x i8] c"Program panicked at ./source.spice:34:25: active field mismatch on union field access 'i'\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [12 x i8] c"Double: %f\0A\00", align 4, !dbg !5
@anon.string.3 = private unnamed_addr constant [91 x i8] c"Program panicked at ./source.spice:36:28: active field mismatch on union field access 'd'\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [9 x i8] c"Sum: %d\0A\00", align 4, !dbg !10
@printf.str.3 = private unnamed_addr constant [15 x i8] c"Boxes: %d, %f\0A\00", align 4, !dbg !13
@anon.string.4 = private unnamed_addr constant [95 x i8] c"Program panicked at ./source.spice:44:31: active field mismatch on union field access 'value'\0A\00", align 4
@anon.string.5 = private unnamed_addr constant [95 x i8] c"Program panicked at ./source.spice:44:45: active field mismatch on union field access 'value'\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [19 x i8] c"Fallbacks: %d, %d\0A\00", align 4, !dbg !16
@anon.string.6 = private unnamed_addr constant [98 x i8] c"Program panicked at ./source.spice:47:35: active field mismatch on union field access 'fallback'\0A\00", align 4
@anon.string.7 = private unnamed_addr constant [98 x i8] c"Program panicked at ./source.spice:47:52: active field mismatch on union field access 'fallback'\0A\00", align 4
@printf.str.5 = private unnamed_addr constant [18 x i8] c"Holder value: %d\0A\00", align 4, !dbg !19
@anon.string.8 = private unnamed_addr constant [95 x i8] c"Program panicked at ./source.spice:51:34: active field mismatch on union field access 'value'\0A\00", align 4
@printf.str.6 = private unnamed_addr constant [21 x i8] c"Holder fallback: %d\0A\00", align 4, !dbg !22
@anon.string.9 = private unnamed_addr constant [98 x i8] c"Program panicked at ./source.spice:54:37: active field mismatch on union field access 'fallback'\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_Z3sumRK5Value(ptr noundef %0) #0 !dbg !32 {
  %value = alloca ptr, align 8
    #dbg_declare(ptr %value, !51, !DIExpression(), !52)
  store ptr %0, ptr %value, align 8, !dbg !52
  %2 = load ptr, ptr %value, align 8, !dbg !53
  %3 = load i32, ptr %2, align 4, !dbg !53
  %4 = icmp eq i32 %3, 3, !dbg !53
  br i1 %4, label %union.tag.ok.L28, label %union.tag.panic.L28, !dbg !53, !prof !54

union.tag.panic.L28:                              ; preds = %1
  %5 = load ptr, ptr @stderr, align 8, !dbg !53
  %6 = call i32 (ptr, ptr, ...) @fprintf(ptr %5, ptr @anon.string.0), !dbg !53
  call void @exit(i32 1), !dbg !53
  unreachable, !dbg !53

union.tag.ok.L28:                                 ; preds = %1
  %vec.addr = getelementptr inbounds nuw %union.Value, ptr %2, i32 0, i32 2, !dbg !53
  %x.addr = getelementptr inbounds %struct.Vec2, ptr %vec.addr, i64 0, i32 0, !dbg !53
  %7 = load ptr, ptr %value, align 8, !dbg !55
  %8 = load i32, ptr %7, align 4, !dbg !55
  %9 = icmp eq i32 %8, 3, !dbg !55
  br i1 %9, label %union.tag.ok.L282, label %union.tag.panic.L281, !dbg !55, !prof !54

union.tag.panic.L281:                             ; preds = %union.tag.ok.L28
  %10 = load ptr, ptr @stderr, align 8, !dbg !55
  %11 = call i32 (ptr, ptr, ...) @fprintf(ptr %10, ptr @anon.string.1), !dbg !55
  call void @exit(i32 1), !dbg !55
  unreachable, !dbg !55

union.tag.ok.L282:                                ; preds = %union.tag.ok.L28
  %vec.addr3 = getelementptr inbounds nuw %union.Value, ptr %7, i32 0, i32 2, !dbg !55
  %y.addr = getelementptr inbounds %struct.Vec2, ptr %vec.addr3, i64 0, i32 1, !dbg !55
  %12 = load i32, ptr %y.addr, align 4, !dbg !55
  %13 = load i32, ptr %x.addr, align 4, !dbg !55
  %14 = add nsw i32 %13, %12, !dbg !55
  ret i32 %14, !dbg !56
}

; Function Attrs: nofree
declare noundef i32 @fprintf(ptr noundef captures(none), ptr noundef readonly captures(none), ...) local_unnamed_addr #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #2

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #3 !dbg !57 {
  %value = alloca %union.Value, align 8
  %intBox = alloca %union.Box, align 8
  %doubleBox = alloca %union.Box.0, align 8
  %holder = alloca %struct.Holder, align 8
    #dbg_declare(ptr %value, !61, !DIExpression(), !83)
  store %union.Value zeroinitializer, ptr %value, align 8, !dbg !83
  store i32 1, ptr %value, align 4, !dbg !84
  %i.addr = getelementptr inbounds nuw %union.Value, ptr %value, i32 0, i32 2, !dbg !84
  store i32 42, ptr %i.addr, align 4, !dbg !85
  %1 = load i32, ptr %value, align 4, !dbg !86
  %2 = icmp eq i32 %1, 1, !dbg !86
  br i1 %2, label %union.tag.ok.L34, label %union.tag.panic.L34, !dbg !86, !prof !54

union.tag.panic.L34:                              ; preds = %0
  %3 = load ptr, ptr @stderr, align 8, !dbg !86
  %4 = call i32 (ptr, ptr, ...) @fprintf(ptr %3, ptr @anon.string.2), !dbg !86
  call void @exit(i32 1), !dbg !86
  unreachable, !dbg !86

union.tag.ok.L34:                                 ; preds = %0
  %i.addr1 = getelementptr inbounds nuw %union.Value, ptr %value, i32 0, i32 2, !dbg !86
  %5 = load i32, ptr %i.addr1, align 4, !dbg !86
  %6 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %5), !dbg !86
  store i32 2, ptr %value, align 4, !dbg !87
  %d.addr = getelementptr inbounds nuw %union.Value, ptr %value, i32 0, i32 2, !dbg !87
  store double 3.500000e+00, ptr %d.addr, align 8, !dbg !88
  %7 = load i32, ptr %value, align 4, !dbg !89
  %8 = icmp eq i32 %7, 2, !dbg !89
  br i1 %8, label %union.tag.ok.L36, label %union.tag.panic.L36, !dbg !89, !prof !54

union.tag.panic.L36:                              ; preds = %union.tag.ok.L34
  %9 = load ptr, ptr @stderr, align 8, !dbg !89
  %10 = call i32 (ptr, ptr, ...) @fprintf(ptr %9, ptr @anon.string.3), !dbg !89
  call void @exit(i32 1), !dbg !89
  unreachable, !dbg !89

union.tag.ok.L36:                                 ; preds = %union.tag.ok.L34
  %d.addr2 = getelementptr inbounds nuw %union.Value, ptr %value, i32 0, i32 2, !dbg !89
  %11 = load double, ptr %d.addr2, align 8, !dbg !89
  %12 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, double noundef %11), !dbg !89
  store i32 3, ptr %value, align 4, !dbg !90
  %vec.addr = getelementptr inbounds nuw %union.Value, ptr %value, i32 0, i32 2, !dbg !90
  store %struct.Vec2 { i32 1, i32 2 }, ptr %vec.addr, align 4, !dbg !91
  %13 = call noundef i32 @_Z3sumRK5Value(ptr noundef %value), !dbg !92
  %14 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %13), !dbg !92
    #dbg_declare(ptr %intBox, !62, !DIExpression(), !93)
  store %union.Box zeroinitializer, ptr %intBox, align 8, !dbg !93
  store i32 1, ptr %intBox, align 4, !dbg !94
  %value.addr = getelementptr inbounds nuw %union.Box, ptr %intBox, i32 0, i32 2, !dbg !94
  store i32 7, ptr %value.addr, align 4, !dbg !95
    #dbg_declare(ptr %doubleBox, !68, !DIExpression(), !96)
  store %union.Box.0 zeroinitializer, ptr %doubleBox, align 8, !dbg !96
  store i32 1, ptr %doubleBox, align 4, !dbg !97
  %value.addr3 = getelementptr inbounds nuw %union.Box.0, ptr %doubleBox, i32 0, i32 2, !dbg !97
  store double 2.500000e+00, ptr %value.addr3, align 8, !dbg !98
  %15 = load i32, ptr %intBox, align 4, !dbg !99
  %16 = icmp eq i32 %15, 1, !dbg !99
  br i1 %16, label %union.tag.ok.L44, label %union.tag.panic.L44, !dbg !99, !prof !54

union.tag.panic.L44:                              ; preds = %union.tag.ok.L36
  %17 = load ptr, ptr @stderr, align 8, !dbg !99
  %18 = call i32 (ptr, ptr, ...) @fprintf(ptr %17, ptr @anon.string.4), !dbg !99
  call void @exit(i32 1), !dbg !99
  unreachable, !dbg !99

union.tag.ok.L44:                                 ; preds = %union.tag.ok.L36
  %value.addr4 = getelementptr inbounds nuw %union.Box, ptr %intBox, i32 0, i32 2, !dbg !99
  %19 = load i32, ptr %value.addr4, align 4, !dbg !99
  %20 = load i32, ptr %doubleBox, align 4, !dbg !100
  %21 = icmp eq i32 %20, 1, !dbg !100
  br i1 %21, label %union.tag.ok.L446, label %union.tag.panic.L445, !dbg !100, !prof !54

union.tag.panic.L445:                             ; preds = %union.tag.ok.L44
  %22 = load ptr, ptr @stderr, align 8, !dbg !100
  %23 = call i32 (ptr, ptr, ...) @fprintf(ptr %22, ptr @anon.string.5), !dbg !100
  call void @exit(i32 1), !dbg !100
  unreachable, !dbg !100

union.tag.ok.L446:                                ; preds = %union.tag.ok.L44
  %value.addr7 = getelementptr inbounds nuw %union.Box.0, ptr %doubleBox, i32 0, i32 2, !dbg !100
  %24 = load double, ptr %value.addr7, align 8, !dbg !100
  %25 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 noundef %19, double noundef %24), !dbg !100
  store i32 2, ptr %intBox, align 4, !dbg !101
  %fallback.addr = getelementptr inbounds nuw %union.Box, ptr %intBox, i32 0, i32 2, !dbg !101
  store i64 8, ptr %fallback.addr, align 8, !dbg !102
  store i32 2, ptr %doubleBox, align 4, !dbg !103
  %fallback.addr8 = getelementptr inbounds nuw %union.Box.0, ptr %doubleBox, i32 0, i32 2, !dbg !103
  store i64 9, ptr %fallback.addr8, align 8, !dbg !104
  %26 = load i32, ptr %intBox, align 4, !dbg !105
  %27 = icmp eq i32 %26, 2, !dbg !105
  br i1 %27, label %union.tag.ok.L47, label %union.tag.panic.L47, !dbg !105, !prof !54

union.tag.panic.L47:                              ; preds = %union.tag.ok.L446
  %28 = load ptr, ptr @stderr, align 8, !dbg !105
  %29 = call i32 (ptr, ptr, ...) @fprintf(ptr %28, ptr @anon.string.6), !dbg !105
  call void @exit(i32 1), !dbg !105
  unreachable, !dbg !105

union.tag.ok.L47:                                 ; preds = %union.tag.ok.L446
  %fallback.addr9 = getelementptr inbounds nuw %union.Box, ptr %intBox, i32 0, i32 2, !dbg !105
  %30 = load i64, ptr %fallback.addr9, align 8, !dbg !105
  %31 = load i32, ptr %doubleBox, align 4, !dbg !106
  %32 = icmp eq i32 %31, 2, !dbg !106
  br i1 %32, label %union.tag.ok.L4711, label %union.tag.panic.L4710, !dbg !106, !prof !54

union.tag.panic.L4710:                            ; preds = %union.tag.ok.L47
  %33 = load ptr, ptr @stderr, align 8, !dbg !106
  %34 = call i32 (ptr, ptr, ...) @fprintf(ptr %33, ptr @anon.string.7), !dbg !106
  call void @exit(i32 1), !dbg !106
  unreachable, !dbg !106

union.tag.ok.L4711:                               ; preds = %union.tag.ok.L47
  %fallback.addr12 = getelementptr inbounds nuw %union.Box.0, ptr %doubleBox, i32 0, i32 2, !dbg !106
  %35 = load i64, ptr %fallback.addr12, align 8, !dbg !106
  %36 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i64 noundef %30, i64 noundef %35), !dbg !106
    #dbg_declare(ptr %holder, !73, !DIExpression(), !107)
  store %struct.Holder zeroinitializer, ptr %holder, align 8, !dbg !107
  %box.addr = getelementptr inbounds %struct.Holder, ptr %holder, i64 0, i32 1, !dbg !108
  store i32 1, ptr %box.addr, align 4, !dbg !108
  %value.addr13 = getelementptr inbounds nuw %union.Box.1, ptr %box.addr, i32 0, i32 2, !dbg !108
  store i16 12, ptr %value.addr13, align 2, !dbg !109
  %box.addr14 = getelementptr inbounds %struct.Holder, ptr %holder, i64 0, i32 1, !dbg !110
  %37 = load i32, ptr %box.addr14, align 4, !dbg !110
  %38 = icmp eq i32 %37, 1, !dbg !110
  br i1 %38, label %union.tag.ok.L51, label %union.tag.panic.L51, !dbg !110, !prof !54

union.tag.panic.L51:                              ; preds = %union.tag.ok.L4711
  %39 = load ptr, ptr @stderr, align 8, !dbg !110
  %40 = call i32 (ptr, ptr, ...) @fprintf(ptr %39, ptr @anon.string.8), !dbg !110
  call void @exit(i32 1), !dbg !110
  unreachable, !dbg !110

union.tag.ok.L51:                                 ; preds = %union.tag.ok.L4711
  %value.addr15 = getelementptr inbounds nuw %union.Box.1, ptr %box.addr14, i32 0, i32 2, !dbg !110
  %41 = load i16, ptr %value.addr15, align 2, !dbg !110
  %42 = sext i16 %41 to i32, !dbg !110
  %43 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.5, i32 noundef %42), !dbg !110
  %box.addr16 = getelementptr inbounds %struct.Holder, ptr %holder, i64 0, i32 1, !dbg !111
  store i32 2, ptr %box.addr16, align 4, !dbg !111
  %fallback.addr17 = getelementptr inbounds nuw %union.Box.1, ptr %box.addr16, i32 0, i32 2, !dbg !111
  store i64 123, ptr %fallback.addr17, align 8, !dbg !112
  %value.addr18 = getelementptr inbounds %struct.Holder, ptr %holder, i64 0, i32 0, !dbg !113
  store i32 4, ptr %value.addr18, align 4, !dbg !113
  %next.addr = getelementptr inbounds nuw %union.Value, ptr %value.addr18, i32 0, i32 2, !dbg !113
  store ptr %value, ptr %next.addr, align 8, !dbg !114
  %box.addr19 = getelementptr inbounds %struct.Holder, ptr %holder, i64 0, i32 1, !dbg !115
  %44 = load i32, ptr %box.addr19, align 4, !dbg !115
  %45 = icmp eq i32 %44, 2, !dbg !115
  br i1 %45, label %union.tag.ok.L54, label %union.tag.panic.L54, !dbg !115, !prof !54

union.tag.panic.L54:                              ; preds = %union.tag.ok.L51
  %46 = load ptr, ptr @stderr, align 8, !dbg !115
  %47 = call i32 (ptr, ptr, ...) @fprintf(ptr %46, ptr @anon.string.9), !dbg !115
  call void @exit(i32 1), !dbg !115
  unreachable, !dbg !115

union.tag.ok.L54:                                 ; preds = %union.tag.ok.L51
  %fallback.addr20 = getelementptr inbounds nuw %union.Box.1, ptr %box.addr19, i32 0, i32 2, !dbg !115
  %48 = load i64, ptr %fallback.addr20, align 8, !dbg !115
  %49 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.6, i64 noundef %48), !dbg !115
  ret i32 0, !dbg !116
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #4

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree }
attributes #2 = { cold noreturn nounwind }
attributes #3 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #4 = { nofree nounwind }

!llvm.module.flags = !{!25, !26, !27, !28, !29, !30}
!llvm.ident = !{!31}
!llvm.dbg.cu = !{!2}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "printf.str.0", linkageName: "printf.str.0", scope: !2, file: !3, line: 34, type: !12, isLocal: true, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !3, producer: "spice version dev [self-hosted] (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false)
!3 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/instrumentation/success-dbg-info-union")
!4 = !{!0, !5, !10, !13, !16, !19, !22}
!5 = !DIGlobalVariableExpression(var: !6, expr: !DIExpression())
!6 = distinct !DIGlobalVariable(name: "printf.str.1", linkageName: "printf.str.1", scope: !2, file: !3, line: 36, type: !7, isLocal: true, isDefinition: true)
!7 = !DICompositeType(tag: DW_TAG_array_type, baseType: !8, size: 96, elements: !9)
!8 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!9 = !{}
!10 = !DIGlobalVariableExpression(var: !11, expr: !DIExpression())
!11 = distinct !DIGlobalVariable(name: "printf.str.2", linkageName: "printf.str.2", scope: !2, file: !3, line: 38, type: !12, isLocal: true, isDefinition: true)
!12 = !DICompositeType(tag: DW_TAG_array_type, baseType: !8, size: 72, elements: !9)
!13 = !DIGlobalVariableExpression(var: !14, expr: !DIExpression())
!14 = distinct !DIGlobalVariable(name: "printf.str.3", linkageName: "printf.str.3", scope: !2, file: !3, line: 44, type: !15, isLocal: true, isDefinition: true)
!15 = !DICompositeType(tag: DW_TAG_array_type, baseType: !8, size: 120, elements: !9)
!16 = !DIGlobalVariableExpression(var: !17, expr: !DIExpression())
!17 = distinct !DIGlobalVariable(name: "printf.str.4", linkageName: "printf.str.4", scope: !2, file: !3, line: 47, type: !18, isLocal: true, isDefinition: true)
!18 = !DICompositeType(tag: DW_TAG_array_type, baseType: !8, size: 152, elements: !9)
!19 = !DIGlobalVariableExpression(var: !20, expr: !DIExpression())
!20 = distinct !DIGlobalVariable(name: "printf.str.5", linkageName: "printf.str.5", scope: !2, file: !3, line: 51, type: !21, isLocal: true, isDefinition: true)
!21 = !DICompositeType(tag: DW_TAG_array_type, baseType: !8, size: 144, elements: !9)
!22 = !DIGlobalVariableExpression(var: !23, expr: !DIExpression())
!23 = distinct !DIGlobalVariable(name: "printf.str.6", linkageName: "printf.str.6", scope: !2, file: !3, line: 54, type: !24, isLocal: true, isDefinition: true)
!24 = !DICompositeType(tag: DW_TAG_array_type, baseType: !8, size: 168, elements: !9)
!25 = !{i32 8, !"PIC Level", i32 2}
!26 = !{i32 7, !"PIE Level", i32 2}
!27 = !{i32 7, !"uwtable", i32 2}
!28 = !{i32 7, !"frame-pointer", i32 0}
!29 = !{i32 7, !"Dwarf Version", i32 5}
!30 = !{i32 2, !"Debug Info Version", i32 3}
!31 = !{!"spice version dev [self-hosted] (https://github.com/spicelang/spice)"}
!32 = distinct !DISubprogram(name: "sum", linkageName: "_Z3sumRK5Value", scope: !3, file: !3, line: 27, type: !33, scopeLine: 27, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !50)
!33 = !DISubroutineType(types: !34)
!34 = !{!35, !36}
!35 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!36 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !37)
!37 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !38)
!38 = !DICompositeType(tag: DW_TAG_union_type, name: "Value", scope: !3, file: !3, line: 10, size: 128, align: 64, elements: !39, identifier: "union.Value")
!39 = !{!40, !41, !43, !48}
!40 = !DIDerivedType(tag: DW_TAG_member, name: "i", scope: !38, file: !3, line: 11, baseType: !35, size: 32, offset: 64)
!41 = !DIDerivedType(tag: DW_TAG_member, name: "d", scope: !38, file: !3, line: 12, baseType: !42, size: 64, offset: 64)
!42 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!43 = !DIDerivedType(tag: DW_TAG_member, name: "vec", scope: !38, file: !3, line: 13, baseType: !44, size: 64, align: 32, offset: 64)
!44 = !DICompositeType(tag: DW_TAG_structure_type, name: "Vec2", scope: !3, file: !3, line: 5, size: 64, align: 32, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !45, identifier: "struct.Vec2")
!45 = !{!46, !47}
!46 = !DIDerivedType(tag: DW_TAG_member, name: "x", scope: !44, file: !3, line: 6, baseType: !35, size: 32)
!47 = !DIDerivedType(tag: DW_TAG_member, name: "y", scope: !44, file: !3, line: 7, baseType: !35, size: 32, offset: 32)
!48 = !DIDerivedType(tag: DW_TAG_member, name: "next", scope: !38, file: !3, line: 14, baseType: !49, size: 64, offset: 64)
!49 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !38, size: 64, dwarfAddressSpace: 0)
!50 = !{!51}
!51 = !DILocalVariable(name: "value", arg: 1, scope: !32, file: !3, line: 27, type: !36)
!52 = !DILocation(line: 27, column: 12, scope: !32)
!53 = !DILocation(line: 28, column: 12, scope: !32)
!54 = !{!"branch_weights", i32 1048575, i32 1}
!55 = !DILocation(line: 28, column: 26, scope: !32)
!56 = !DILocation(line: 29, column: 1, scope: !32)
!57 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !3, file: !3, line: 31, type: !58, scopeLine: 31, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !60)
!58 = !DISubroutineType(types: !59)
!59 = !{!35}
!60 = !{!61, !62, !68, !73}
!61 = !DILocalVariable(name: "value", scope: !57, file: !3, line: 32, type: !38)
!62 = !DILocalVariable(name: "intBox", scope: !57, file: !3, line: 40, type: !63)
!63 = !DICompositeType(tag: DW_TAG_union_type, name: "Box<int>", scope: !3, file: !3, line: 17, size: 128, align: 64, elements: !64, identifier: "union.Box")
!64 = !{!65, !66}
!65 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !63, file: !3, line: 18, baseType: !35, size: 32, offset: 64)
!66 = !DIDerivedType(tag: DW_TAG_member, name: "fallback", scope: !63, file: !3, line: 19, baseType: !67, size: 64, offset: 64)
!67 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!68 = !DILocalVariable(name: "doubleBox", scope: !57, file: !3, line: 42, type: !69)
!69 = !DICompositeType(tag: DW_TAG_union_type, name: "Box<double>", scope: !3, file: !3, line: 17, size: 128, align: 64, elements: !70, identifier: "union.Box")
!70 = !{!71, !72}
!71 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !69, file: !3, line: 18, baseType: !42, size: 64, offset: 64)
!72 = !DIDerivedType(tag: DW_TAG_member, name: "fallback", scope: !69, file: !3, line: 19, baseType: !67, size: 64, offset: 64)
!73 = !DILocalVariable(name: "holder", scope: !57, file: !3, line: 49, type: !74)
!74 = !DICompositeType(tag: DW_TAG_structure_type, name: "Holder", scope: !3, file: !3, line: 22, size: 256, align: 64, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !75, identifier: "struct.Holder")
!75 = !{!76, !77}
!76 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !74, file: !3, line: 23, baseType: !38, size: 128, align: 64)
!77 = !DIDerivedType(tag: DW_TAG_member, name: "box", scope: !74, file: !3, line: 24, baseType: !78, size: 128, align: 64, offset: 128)
!78 = !DICompositeType(tag: DW_TAG_union_type, name: "Box<short>", scope: !3, file: !3, line: 17, size: 128, align: 64, elements: !79, identifier: "union.Box")
!79 = !{!80, !82}
!80 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !78, file: !3, line: 18, baseType: !81, size: 16, offset: 64)
!81 = !DIBasicType(name: "short", size: 16, encoding: DW_ATE_signed)
!82 = !DIDerivedType(tag: DW_TAG_member, name: "fallback", scope: !78, file: !3, line: 19, baseType: !67, size: 64, offset: 64)
!83 = !DILocation(line: 32, column: 5, scope: !57)
!84 = !DILocation(line: 33, column: 5, scope: !57)
!85 = !DILocation(line: 33, column: 15, scope: !57)
!86 = !DILocation(line: 34, column: 25, scope: !57)
!87 = !DILocation(line: 35, column: 5, scope: !57)
!88 = !DILocation(line: 35, column: 15, scope: !57)
!89 = !DILocation(line: 36, column: 28, scope: !57)
!90 = !DILocation(line: 37, column: 5, scope: !57)
!91 = !DILocation(line: 37, column: 26, scope: !57)
!92 = !DILocation(line: 38, column: 29, scope: !57)
!93 = !DILocation(line: 40, column: 5, scope: !57)
!94 = !DILocation(line: 41, column: 5, scope: !57)
!95 = !DILocation(line: 41, column: 20, scope: !57)
!96 = !DILocation(line: 42, column: 5, scope: !57)
!97 = !DILocation(line: 43, column: 5, scope: !57)
!98 = !DILocation(line: 43, column: 23, scope: !57)
!99 = !DILocation(line: 44, column: 31, scope: !57)
!100 = !DILocation(line: 44, column: 45, scope: !57)
!101 = !DILocation(line: 45, column: 5, scope: !57)
!102 = !DILocation(line: 45, column: 23, scope: !57)
!103 = !DILocation(line: 46, column: 5, scope: !57)
!104 = !DILocation(line: 46, column: 26, scope: !57)
!105 = !DILocation(line: 47, column: 35, scope: !57)
!106 = !DILocation(line: 47, column: 52, scope: !57)
!107 = !DILocation(line: 49, column: 5, scope: !57)
!108 = !DILocation(line: 50, column: 5, scope: !57)
!109 = !DILocation(line: 50, column: 24, scope: !57)
!110 = !DILocation(line: 51, column: 34, scope: !57)
!111 = !DILocation(line: 52, column: 5, scope: !57)
!112 = !DILocation(line: 52, column: 27, scope: !57)
!113 = !DILocation(line: 53, column: 5, scope: !57)
!114 = !DILocation(line: 53, column: 26, scope: !57)
!115 = !DILocation(line: 54, column: 37, scope: !57)
!116 = !DILocation(line: 55, column: 1, scope: !57)
