; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [13 x i8] c"Counter: %d\0A\00", align 4, !dbg !0
@printf.str.1 = private unnamed_addr constant [19 x i8] c"Doubled twice: %d\0A\00", align 4, !dbg !5
@printf.str.2 = private unnamed_addr constant [13 x i8] c"Is even: %d\0A\00", align 4, !dbg !10
@printf.str.3 = private unnamed_addr constant [25 x i8] c"Counter after reset: %d\0A\00", align 4, !dbg !13
@printf.str.4 = private unnamed_addr constant [21 x i8] c"Counter via ptr: %d\0A\00", align 4, !dbg !16

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_Z10applyTwicePFiiEi({ ptr, ptr, i64 } noundef %0, i32 noundef %1) #0 !dbg !26 {
  %fct = alloca { ptr, ptr, i64 }, align 8
  %value = alloca i32, align 4
    #dbg_declare(ptr %fct, !39, !DIExpression(), !41)
  store { ptr, ptr, i64 } %0, ptr %fct, align 8, !dbg !41
    #dbg_declare(ptr %value, !40, !DIExpression(), !42)
  store i32 %1, ptr %value, align 4, !dbg !42
  %3 = load i32, ptr %value, align 4, !dbg !43
  %4 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fct, i32 0, i32 1, !dbg !43
  %captures = load ptr, ptr %4, align 8, !dbg !43
  %fct1 = load ptr, ptr %fct, align 8, !dbg !43
  %5 = call i32 %fct1(i32 %3, ptr %captures), !dbg !43
  %6 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fct, i32 0, i32 1, !dbg !43
  %captures2 = load ptr, ptr %6, align 8, !dbg !43
  %fct3 = load ptr, ptr %fct, align 8, !dbg !43
  %7 = call i32 %fct3(i32 %5, ptr %captures2), !dbg !43
  ret i32 %7, !dbg !44
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #1 !dbg !45 {
  %counter = alloca i32, align 4
  %offset = alloca i64, align 8
  %captures = alloca { ptr, i64 }, align 8
  %fat.ptr = alloca { ptr, ptr, i64 }, align 8
  %increment = alloca { ptr, ptr, i64 }, align 8
  %fat.ptr2 = alloca { ptr, ptr, i64 }, align 8
  %doubleFct = alloca { ptr, ptr, i64 }, align 8
  %fat.ptr3 = alloca { ptr, ptr, i64 }, align 8
  %isEven = alloca { ptr, ptr, i64 }, align 8
  %fat.ptr6 = alloca { ptr, ptr, i64 }, align 8
  %reset = alloca { ptr, ptr, i64 }, align 8
  %counterPtr = alloca ptr, align 8
  %fat.ptr9 = alloca { ptr, ptr, i64 }, align 8
  %printViaPtr = alloca { ptr, ptr, i64 }, align 8
    #dbg_declare(ptr %counter, !49, !DIExpression(), !73)
  store i32 10, ptr %counter, align 4, !dbg !73
    #dbg_declare(ptr %offset, !50, !DIExpression(), !74)
  store i64 5, ptr %offset, align 8, !dbg !74
  store ptr %counter, ptr %captures, align 8, !dbg !75
  %1 = load i64, ptr %offset, align 8, !dbg !75
  %2 = getelementptr inbounds nuw { ptr, i64 }, ptr %captures, i32 0, i32 1, !dbg !75
  store i64 %1, ptr %2, align 8, !dbg !75
  store ptr @_Z15lambda.L10C21.0v, ptr %fat.ptr, align 8, !dbg !75
  %3 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 1, !dbg !75
  store ptr %captures, ptr %3, align 8, !dbg !75
  %4 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 2, !dbg !75
  store i64 16, ptr %4, align 8, !dbg !75
    #dbg_declare(ptr %increment, !58, !DIExpression(), !75)
  %5 = load { ptr, ptr, i64 }, ptr %fat.ptr, align 8, !dbg !75
  store { ptr, ptr, i64 } %5, ptr %increment, align 8, !dbg !75
  %6 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %increment, i32 0, i32 1, !dbg !76
  %captures1 = load ptr, ptr %6, align 8, !dbg !76
  %fct = load ptr, ptr %increment, align 8, !dbg !76
  call void %fct(ptr %captures1), !dbg !76
  %7 = load i32, ptr %counter, align 4, !dbg !77
  %8 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %7), !dbg !77
  store ptr @_Z15lambda.L18C29.0i, ptr %fat.ptr2, align 8, !dbg !78
  %9 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr2, i32 0, i32 1, !dbg !78
  store ptr null, ptr %9, align 8, !dbg !78
  %10 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr2, i32 0, i32 2, !dbg !78
  store i64 0, ptr %10, align 8, !dbg !78
    #dbg_declare(ptr %doubleFct, !60, !DIExpression(), !78)
  %11 = load { ptr, ptr, i64 }, ptr %fat.ptr2, align 8, !dbg !78
  store { ptr, ptr, i64 } %11, ptr %doubleFct, align 8, !dbg !78
  %12 = load { ptr, ptr, i64 }, ptr %doubleFct, align 8, !dbg !79
  %13 = load i32, ptr %counter, align 4, !dbg !80
  %14 = call noundef i32 @_Z10applyTwicePFiiEi({ ptr, ptr, i64 } noundef %12, i32 noundef %13), !dbg !80
  %15 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %14), !dbg !80
  store ptr @_Z15lambda.L24C27.0i, ptr %fat.ptr3, align 8, !dbg !81
  %16 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr3, i32 0, i32 1, !dbg !81
  store ptr null, ptr %16, align 8, !dbg !81
  %17 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr3, i32 0, i32 2, !dbg !81
  store i64 0, ptr %17, align 8, !dbg !81
    #dbg_declare(ptr %isEven, !62, !DIExpression(), !81)
  %18 = load { ptr, ptr, i64 }, ptr %fat.ptr3, align 8, !dbg !81
  store { ptr, ptr, i64 } %18, ptr %isEven, align 8, !dbg !81
  %19 = load i32, ptr %counter, align 4, !dbg !82
  %20 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %isEven, i32 0, i32 1, !dbg !82
  %captures4 = load ptr, ptr %20, align 8, !dbg !82
  %fct5 = load ptr, ptr %isEven, align 8, !dbg !82
  %21 = call i1 %fct5(i32 %19, ptr %captures4), !dbg !82
  %22 = zext i1 %21 to i32, !dbg !82
  %23 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %22), !dbg !82
  store ptr @_Z15lambda.L28C17.0v, ptr %fat.ptr6, align 8, !dbg !83
  %24 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr6, i32 0, i32 1, !dbg !83
  store ptr %counter, ptr %24, align 8, !dbg !83
  %25 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr6, i32 0, i32 2, !dbg !83
  store i64 0, ptr %25, align 8, !dbg !83
    #dbg_declare(ptr %reset, !66, !DIExpression(), !83)
  %26 = load { ptr, ptr, i64 }, ptr %fat.ptr6, align 8, !dbg !83
  store { ptr, ptr, i64 } %26, ptr %reset, align 8, !dbg !83
  %27 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %reset, i32 0, i32 1, !dbg !84
  %captures7 = load ptr, ptr %27, align 8, !dbg !84
  %fct8 = load ptr, ptr %reset, align 8, !dbg !84
  call void %fct8(ptr %captures7), !dbg !84
  %28 = load i32, ptr %counter, align 4, !dbg !85
  %29 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 noundef %28), !dbg !85
    #dbg_declare(ptr %counterPtr, !67, !DIExpression(), !86)
  store ptr %counter, ptr %counterPtr, align 8, !dbg !86
  %30 = load ptr, ptr %counterPtr, align 8, !dbg !87
  store ptr @_Z15lambda.L36C23.0v, ptr %fat.ptr9, align 8, !dbg !87
  %31 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr9, i32 0, i32 1, !dbg !87
  store ptr %30, ptr %31, align 8, !dbg !87
  %32 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr9, i32 0, i32 2, !dbg !87
  store i64 0, ptr %32, align 8, !dbg !87
    #dbg_declare(ptr %printViaPtr, !72, !DIExpression(), !87)
  %33 = load { ptr, ptr, i64 }, ptr %fat.ptr9, align 8, !dbg !87
  store { ptr, ptr, i64 } %33, ptr %printViaPtr, align 8, !dbg !87
  %34 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %printViaPtr, i32 0, i32 1, !dbg !88
  %captures10 = load ptr, ptr %34, align 8, !dbg !88
  %fct11 = load ptr, ptr %printViaPtr, align 8, !dbg !88
  call void %fct11(ptr %captures10), !dbg !88
  ret i32 0, !dbg !89
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z15lambda.L10C21.0v(ptr noundef nonnull dereferenceable(8) %0) #0 !dbg !90 {
  %captures = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8, !dbg !97
  %2 = load ptr, ptr %captures, align 8, !dbg !97
    #dbg_declare(ptr %captures, !95, !DIExpression(DW_OP_deref, DW_OP_plus_uconst, 0, DW_OP_deref), !97)
  %offset = getelementptr inbounds nuw { ptr, i64 }, ptr %2, i32 0, i32 1, !dbg !97
    #dbg_declare(ptr %captures, !96, !DIExpression(DW_OP_deref, DW_OP_plus_uconst, 8), !97)
  %3 = load ptr, ptr %2, align 8, !dbg !98
  %4 = load i32, ptr %3, align 4, !dbg !98
  %5 = add nsw i32 %4, 1, !dbg !98
  store i32 %5, ptr %3, align 4, !dbg !98
  %6 = load i64, ptr %offset, align 8, !dbg !99
  %7 = trunc i64 %6 to i32, !dbg !99
  %8 = load ptr, ptr %2, align 8, !dbg !99
  %9 = load i32, ptr %8, align 4, !dbg !99
  %10 = add nsw i32 %9, %7, !dbg !99
  store i32 %10, ptr %8, align 4, !dbg !99
  ret void, !dbg !100
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

; Function Attrs: noinline nounwind optnone uwtable
define internal i32 @_Z15lambda.L18C29.0i(i32 %0, ptr %1) #0 !dbg !101 {
  %x = alloca i32, align 4
  %captures = alloca ptr, align 8
    #dbg_declare(ptr %x, !106, !DIExpression(), !107)
  store i32 %0, ptr %x, align 4, !dbg !107
  store ptr %1, ptr %captures, align 8, !dbg !107
  %3 = load i32, ptr %x, align 4, !dbg !108
  %4 = mul nsw i32 %3, 2, !dbg !108
  ret i32 %4, !dbg !109
}

; Function Attrs: noinline nounwind optnone uwtable
define internal i1 @_Z15lambda.L24C27.0i(i32 %0, ptr %1) #0 !dbg !110 {
  %x = alloca i32, align 4
  %captures = alloca ptr, align 8
    #dbg_declare(ptr %x, !116, !DIExpression(), !117)
  store i32 %0, ptr %x, align 4, !dbg !117
  store ptr %1, ptr %captures, align 8, !dbg !117
  %3 = load i32, ptr %x, align 4, !dbg !118
  %4 = srem i32 %3, 2, !dbg !118
  %5 = icmp eq i32 %4, 0, !dbg !119
  ret i1 %5, !dbg !119
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z15lambda.L28C17.0v(ptr noundef nonnull dereferenceable(8) %0) #0 !dbg !120 {
  %captures = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8, !dbg !126
    #dbg_declare(ptr %captures, !125, !DIExpression(DW_OP_deref), !126)
  %2 = load ptr, ptr %captures, align 8, !dbg !127
  store i32 0, ptr %2, align 4, !dbg !128
  ret void, !dbg !129
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z15lambda.L36C23.0v(ptr noundef nonnull dereferenceable(8) %0) #0 !dbg !130 {
  %captures = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8, !dbg !136
    #dbg_declare(ptr %captures, !135, !DIExpression(), !136)
  %2 = load ptr, ptr %captures, align 8, !dbg !137
  %3 = load i32, ptr %2, align 4, !dbg !137
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 noundef %3), !dbg !137
  ret void, !dbg !138
}

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }

!llvm.module.flags = !{!19, !20, !21, !22, !23, !24}
!llvm.ident = !{!25}
!llvm.dbg.cu = !{!2}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "printf.str.0", linkageName: "printf.str.0", scope: !2, file: !3, line: 15, type: !12, isLocal: true, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !3, producer: "spice version dev (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false)
!3 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/instrumentation/success-dbg-info-lambda")
!4 = !{!0, !5, !10, !13, !16}
!5 = !DIGlobalVariableExpression(var: !6, expr: !DIExpression())
!6 = distinct !DIGlobalVariable(name: "printf.str.1", linkageName: "printf.str.1", scope: !2, file: !3, line: 21, type: !7, isLocal: true, isDefinition: true)
!7 = !DICompositeType(tag: DW_TAG_array_type, baseType: !8, size: 152, elements: !9)
!8 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!9 = !{}
!10 = !DIGlobalVariableExpression(var: !11, expr: !DIExpression())
!11 = distinct !DIGlobalVariable(name: "printf.str.2", linkageName: "printf.str.2", scope: !2, file: !3, line: 25, type: !12, isLocal: true, isDefinition: true)
!12 = !DICompositeType(tag: DW_TAG_array_type, baseType: !8, size: 104, elements: !9)
!13 = !DIGlobalVariableExpression(var: !14, expr: !DIExpression())
!14 = distinct !DIGlobalVariable(name: "printf.str.3", linkageName: "printf.str.3", scope: !2, file: !3, line: 32, type: !15, isLocal: true, isDefinition: true)
!15 = !DICompositeType(tag: DW_TAG_array_type, baseType: !8, size: 200, elements: !9)
!16 = !DIGlobalVariableExpression(var: !17, expr: !DIExpression())
!17 = distinct !DIGlobalVariable(name: "printf.str.4", linkageName: "printf.str.4", scope: !2, file: !3, line: 37, type: !18, isLocal: true, isDefinition: true)
!18 = !DICompositeType(tag: DW_TAG_array_type, baseType: !8, size: 168, elements: !9)
!19 = !{i32 8, !"PIC Level", i32 2}
!20 = !{i32 7, !"PIE Level", i32 2}
!21 = !{i32 7, !"uwtable", i32 2}
!22 = !{i32 7, !"frame-pointer", i32 0}
!23 = !{i32 7, !"Dwarf Version", i32 5}
!24 = !{i32 2, !"Debug Info Version", i32 3}
!25 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!26 = distinct !DISubprogram(name: "applyTwice", linkageName: "_Z10applyTwicePFiiEi", scope: !3, file: !3, line: 3, type: !27, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !38)
!27 = !DISubroutineType(types: !28)
!28 = !{!29, !30, !29}
!29 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!30 = !DICompositeType(tag: DW_TAG_structure_type, name: "_lambda", scope: !3, file: !3, size: 192, align: 64, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !31, identifier: "_lambda")
!31 = !{!32, !35, !36}
!32 = !DIDerivedType(tag: DW_TAG_member, name: "fct", scope: !30, file: !3, baseType: !33, size: 64, align: 64)
!33 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !34, size: 64, align: 64, dwarfAddressSpace: 0)
!34 = !DIBasicType(name: "void", encoding: DW_ATE_unsigned)
!35 = !DIDerivedType(tag: DW_TAG_member, name: "captures", scope: !30, file: !3, baseType: !33, size: 64, align: 64, offset: 64)
!36 = !DIDerivedType(tag: DW_TAG_member, name: "captureSize", scope: !30, file: !3, baseType: !37, size: 64, align: 64, offset: 128)
!37 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!38 = !{!39, !40}
!39 = !DILocalVariable(name: "fct", arg: 1, scope: !26, file: !3, line: 3, type: !30)
!40 = !DILocalVariable(name: "value", arg: 2, scope: !26, file: !3, line: 3, type: !29)
!41 = !DILocation(line: 3, column: 19, scope: !26)
!42 = !DILocation(line: 3, column: 36, scope: !26)
!43 = !DILocation(line: 4, column: 20, scope: !26)
!44 = !DILocation(line: 5, column: 1, scope: !26)
!45 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !3, file: !3, line: 7, type: !46, scopeLine: 7, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !48)
!46 = !DISubroutineType(types: !47)
!47 = !{!29}
!48 = !{!49, !50, !53, !58, !59, !60, !61, !62, !63, !66, !67, !69, !72}
!49 = !DILocalVariable(name: "counter", scope: !45, file: !3, line: 8, type: !29)
!50 = !DILocalVariable(name: "offset", scope: !45, file: !3, line: 9, type: !51)
!51 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !52)
!52 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!53 = !DICompositeType(tag: DW_TAG_class_type, scope: !45, file: !3, line: 10, size: 128, align: 64, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !54)
!54 = !{!55, !57}
!55 = !DIDerivedType(tag: DW_TAG_member, name: "counter", scope: !53, file: !3, line: 10, baseType: !56)
!56 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !29)
!57 = !DIDerivedType(tag: DW_TAG_member, name: "offset", scope: !53, file: !3, line: 10, baseType: !51, offset: 64)
!58 = !DILocalVariable(name: "increment", scope: !45, file: !3, line: 10, type: !30)
!59 = !DICompositeType(tag: DW_TAG_class_type, scope: !45, file: !3, line: 18, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !9)
!60 = !DILocalVariable(name: "doubleFct", scope: !45, file: !3, line: 18, type: !30)
!61 = !DICompositeType(tag: DW_TAG_class_type, scope: !45, file: !3, line: 24, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !9)
!62 = !DILocalVariable(name: "isEven", scope: !45, file: !3, line: 24, type: !30)
!63 = !DICompositeType(tag: DW_TAG_class_type, scope: !45, file: !3, line: 28, size: 64, align: 64, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !64)
!64 = !{!65}
!65 = !DIDerivedType(tag: DW_TAG_member, name: "counter", scope: !63, file: !3, line: 28, baseType: !56)
!66 = !DILocalVariable(name: "reset", scope: !45, file: !3, line: 28, type: !30)
!67 = !DILocalVariable(name: "counterPtr", scope: !45, file: !3, line: 35, type: !68)
!68 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !29, size: 64, dwarfAddressSpace: 0)
!69 = !DICompositeType(tag: DW_TAG_class_type, scope: !45, file: !3, line: 36, size: 64, align: 64, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !70)
!70 = !{!71}
!71 = !DIDerivedType(tag: DW_TAG_member, name: "counterPtr", scope: !69, file: !3, line: 36, baseType: !68, size: 64)
!72 = !DILocalVariable(name: "printViaPtr", scope: !45, file: !3, line: 36, type: !30)
!73 = !DILocation(line: 8, column: 19, scope: !45)
!74 = !DILocation(line: 9, column: 25, scope: !45)
!75 = !DILocation(line: 10, column: 21, scope: !45)
!76 = !DILocation(line: 14, column: 5, scope: !45)
!77 = !DILocation(line: 15, column: 29, scope: !45)
!78 = !DILocation(line: 18, column: 29, scope: !45)
!79 = !DILocation(line: 21, column: 46, scope: !45)
!80 = !DILocation(line: 21, column: 57, scope: !45)
!81 = !DILocation(line: 24, column: 27, scope: !45)
!82 = !DILocation(line: 25, column: 36, scope: !45)
!83 = !DILocation(line: 28, column: 17, scope: !45)
!84 = !DILocation(line: 31, column: 5, scope: !45)
!85 = !DILocation(line: 32, column: 41, scope: !45)
!86 = !DILocation(line: 35, column: 24, scope: !45)
!87 = !DILocation(line: 36, column: 23, scope: !45)
!88 = !DILocation(line: 39, column: 5, scope: !45)
!89 = !DILocation(line: 40, column: 1, scope: !45)
!90 = distinct !DISubprogram(name: "lambda.L10C21", linkageName: "_Z15lambda.L10C21.0v", scope: !53, file: !3, line: 10, type: !91, scopeLine: 10, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !94)
!91 = !DISubroutineType(types: !92)
!92 = !{!34, !93}
!93 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !53, size: 64, dwarfAddressSpace: 0)
!94 = !{!95, !96}
!95 = !DILocalVariable(name: "counter", scope: !90, file: !3, line: 8, type: !29)
!96 = !DILocalVariable(name: "offset", scope: !90, file: !3, line: 9, type: !51)
!97 = !DILocation(line: 10, column: 21, scope: !90)
!98 = !DILocation(line: 11, column: 9, scope: !90)
!99 = !DILocation(line: 12, column: 9, scope: !90)
!100 = !DILocation(line: 13, column: 5, scope: !90)
!101 = distinct !DISubprogram(name: "lambda.L18C29", linkageName: "_Z15lambda.L18C29.0i", scope: !59, file: !3, line: 18, type: !102, scopeLine: 18, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !105)
!102 = !DISubroutineType(types: !103)
!103 = !{!29, !104, !29}
!104 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !59, size: 64, dwarfAddressSpace: 0)
!105 = !{!106}
!106 = !DILocalVariable(name: "x", arg: 1, scope: !101, file: !3, line: 18, type: !29)
!107 = !DILocation(line: 18, column: 29, scope: !101)
!108 = !DILocation(line: 19, column: 20, scope: !101)
!109 = !DILocation(line: 20, column: 5, scope: !101)
!110 = distinct !DISubprogram(name: "lambda.L24C27", linkageName: "_Z15lambda.L24C27.0i", scope: !61, file: !3, line: 24, type: !111, scopeLine: 24, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !115)
!111 = !DISubroutineType(types: !112)
!112 = !{!113, !114, !29}
!113 = !DIBasicType(name: "bool", size: 8, encoding: DW_ATE_boolean)
!114 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !61, size: 64, dwarfAddressSpace: 0)
!115 = !{!116}
!116 = !DILocalVariable(name: "x", arg: 1, scope: !110, file: !3, line: 24, type: !29)
!117 = !DILocation(line: 24, column: 27, scope: !110)
!118 = !DILocation(line: 24, column: 42, scope: !110)
!119 = !DILocation(line: 24, column: 47, scope: !110)
!120 = distinct !DISubprogram(name: "lambda.L28C17", linkageName: "_Z15lambda.L28C17.0v", scope: !63, file: !3, line: 28, type: !121, scopeLine: 28, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !124)
!121 = !DISubroutineType(types: !122)
!122 = !{!34, !123}
!123 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !63, size: 64, dwarfAddressSpace: 0)
!124 = !{!125}
!125 = !DILocalVariable(name: "counter", scope: !120, file: !3, line: 8, type: !29)
!126 = !DILocation(line: 28, column: 17, scope: !120)
!127 = !DILocation(line: 29, column: 9, scope: !120)
!128 = !DILocation(line: 29, column: 19, scope: !120)
!129 = !DILocation(line: 30, column: 5, scope: !120)
!130 = distinct !DISubprogram(name: "lambda.L36C23", linkageName: "_Z15lambda.L36C23.0v", scope: !69, file: !3, line: 36, type: !131, scopeLine: 36, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !134)
!131 = !DISubroutineType(types: !132)
!132 = !{!34, !133}
!133 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !69, size: 64, dwarfAddressSpace: 0)
!134 = !{!135}
!135 = !DILocalVariable(name: "counterPtr", scope: !130, file: !3, line: 35, type: !68)
!136 = !DILocation(line: 36, column: 23, scope: !130)
!137 = !DILocation(line: 37, column: 42, scope: !130)
!138 = !DILocation(line: 38, column: 5, scope: !130)
